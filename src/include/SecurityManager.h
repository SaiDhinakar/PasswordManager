#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <cstring>
using namespace std;

// Forward declaration of app struct to match your main.cpp
struct app {
    string app_name;
    map<string, string> user_data;
};

class SecurityManager {
private:
    const string SALT = "Pa$$M@nag3r"; // Salt for additional security
    string masterKey;
    string dataFilePath;

    // Simple XOR encryption/decryption
    string xorCipher(const string& input, const string& key) const {
        string output = input;
        for (size_t i = 0; i < input.length(); i++) {
            output[i] = input[i] ^ key[i % key.length()];
        }
        return output;
    }

    // Convert binary data to hex string for storage
    string toHex(const string& input) const {
        string output;
        static const char hex[] = "0123456789ABCDEF";
        
        for (unsigned char c : input) {
            output.push_back(hex[c >> 4]);
            output.push_back(hex[c & 15]);
        }
        return output;
    }
    
    // Convert hex string back to binary data
    string fromHex(const string& hex) const {
        string output;
        for (size_t i = 0; i < hex.length(); i += 2) {
            string byteString = hex.substr(i, 2);
            char byte = (char) stoi(byteString, nullptr, 16);
            output.push_back(byte);
        }
        return output;
    }

    // Generate a stronger key from master password
    string generateKey(const string& password) const {
        string combined = password + SALT;
        // Simple key stretching (not as secure as proper key derivation)
        for (int i = 0; i < 1000; i++) {
            combined = xorCipher(combined, password);
        }
        return combined;
    }

public:
    SecurityManager(const string& filePath) : dataFilePath(filePath) {}
    
    void setMasterKey(const string& password) {
        masterKey = generateKey(password);
    }
    
    // Encrypt and save data to file
    bool saveData(const map<int, struct app>& apps, const map<string, string>& userCredentials) {
        try {
            ofstream file(dataFilePath, ios::binary);
            if (!file.is_open()) {
                return false;
            }
            
            // First store user credentials
            string userDataStr = "USERS:";
            for (const auto& user : userCredentials) {
                userDataStr += user.first + ":" + user.second + ";";
            }
            
            // Then store password data
            string appDataStr = "APPS:";
            for (const auto& app_pair : apps) {
                appDataStr += to_string(app_pair.first) + ":" + app_pair.second.app_name + ":";
                
                for (const auto& user_data : app_pair.second.user_data) {
                    appDataStr += user_data.first + "=" + user_data.second + ",";
                }
                appDataStr += ";";
            }
            
            // Combine all data
            string allData = userDataStr + "\n" + appDataStr;
            
            // Encrypt the data
            string encryptedData = xorCipher(allData, masterKey);
            
            // Convert to hex for safe storage
            string hexData = toHex(encryptedData);
            
            // Write to file
            file << hexData;
            file.close();
            return true;
        }
        catch(const exception& e) {
            return false;
        }
    }
    
    // Load and decrypt data from file
    bool loadData(map<int, struct app>& apps, map<string, string>& userCredentials) {
        try {
            ifstream file(dataFilePath, ios::binary);
            if (!file.is_open()) {
                return false;
            }
            
            string hexData((istreambuf_iterator<char>(file)),
                                istreambuf_iterator<char>());
            file.close();
            
            if (hexData.empty()) {
                return false;
            }
            
            // Convert from hex
            string encryptedData = fromHex(hexData);
            
            // Decrypt the data
            string decryptedData = xorCipher(encryptedData, masterKey);
            
            // Parse the data
            size_t separator = decryptedData.find('\n');
            if (separator == string::npos) {
                return false;
            }
            
            // Extract user credentials
            string userDataStr = decryptedData.substr(0, separator);
            if (userDataStr.find("USERS:") == 0) {
                userDataStr = userDataStr.substr(6); // Skip "USERS:"
                size_t pos = 0;
                while ((pos = userDataStr.find(';')) != string::npos) {
                    string userEntry = userDataStr.substr(0, pos);
                    size_t colonPos = userEntry.find(':');
                    if (colonPos != string::npos) {
                        string username = userEntry.substr(0, colonPos);
                        string password = userEntry.substr(colonPos + 1);
                        userCredentials[username] = password;
                    }
                    userDataStr.erase(0, pos + 1);
                }
            }
            
            // Extract app data
            string appDataStr = decryptedData.substr(separator + 1);
            if (appDataStr.find("APPS:") == 0) {
                appDataStr = appDataStr.substr(5); // Skip "APPS:"
                
                // Clear existing apps
                apps.clear();
                
                size_t pos = 0;
                while ((pos = appDataStr.find(';')) != string::npos) {
                    string appEntry = appDataStr.substr(0, pos);
                    
                    // Parse app entry
                    size_t firstColon = appEntry.find(':');
                    size_t secondColon = appEntry.find(':', firstColon + 1);
                    
                    if (firstColon != string::npos && secondColon != string::npos) {
                        int appId = stoi(appEntry.substr(0, firstColon));
                        string appName = appEntry.substr(firstColon + 1, secondColon - firstColon - 1);
                        string userData = appEntry.substr(secondColon + 1);
                        
                        struct app newApp;
                        newApp.app_name = appName;
                        
                        // Parse user data
                        size_t commaPos = 0;
                        while ((commaPos = userData.find(',')) != string::npos) {
                            string userEntry = userData.substr(0, commaPos);
                            size_t equalPos = userEntry.find('=');
                            if (equalPos != string::npos) {
                                string username = userEntry.substr(0, equalPos);
                                string password = userEntry.substr(equalPos + 1);
                                newApp.user_data[username] = password;
                            }
                            userData.erase(0, commaPos + 1);
                        }
                        
                        apps[appId] = newApp;
                    }
                    
                    appDataStr.erase(0, pos + 1);
                }
            }
            
            return true;
        }
        catch(const exception& e) {
            return false;
        }
    }
    
    // Check if data file exists
    bool dataFileExists() const {
        ifstream file(dataFilePath);
        return file.good();
    }
    
    // Initialize a new data file with default admin user
    bool initializeDataFile(const string& adminUser, const string& adminPass) {
        map<int, struct app> emptyApps;
        map<string, string> defaultUser;
        defaultUser[adminUser] = adminPass;
        
        setMasterKey(adminPass); // Use admin password as the master key
        return saveData(emptyApps, defaultUser);
    }
    
    // Check if username and password are valid
    bool validateCredentials(const string& username, const string& password) {
        map<int, struct app> unusedApps;
        map<string, string> users;
        
        // Try to load data with the provided password as the key
        setMasterKey(password);
        if (loadData(unusedApps, users)) {
            // Check if the username exists and the password matches
            auto it = users.find(username);
            return (it != users.end() && it->second == password);
        }
        return false;
    }
};

#endif // SECURITY_MANAGER_H