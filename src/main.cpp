#include <iostream>
#include <map>
#include <iomanip>
#include <chrono>
#include <thread>
#include "include/SecurityManager.h"
#include <filesystem>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    // Windows
    system("cls");
#else
    // Unix/Linux/MacOS
    system("clear");
#endif
}

void timer(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
    clearScreen();
}

class PasswordManager {

    private:
        map<int, struct app> apps;
    
        int app_name_exist(string app_name){
            for(auto app : apps){
                if(app.second.app_name  == app_name){
                    return app.first;
                }
            }
            return -1;
        }

    public:
        void addPassword(string app_name, string user_name, string password){
            int app_idx = app_name_exist(app_name); 
            if( app_idx == -1){
                app_idx = apps.size()+1;
                struct app new_app;
                new_app.app_name = app_name;
                apps[app_idx] = new_app;
            }
            apps[app_idx].user_data[user_name] = password;
            cout<<"Password addded successfully."<<std::flush;
            timer(2);
        }

        void updatePassword(string app_name, string user_name, string newPassword){
            int app_idx = app_name_exist(app_name);
            if(app_idx == -1){
                cout<<"App name not found!";
                timer(2);
                return;
            }

            auto user_data = apps[app_idx].user_data.find(user_name);
            
            if(user_data == apps[app_idx].user_data.end()){
                clearScreen();
                cout<<"Username does not exists!"<<std::flush;
                timer(2);
                return;
            }

            apps[app_idx].user_data[user_name] = newPassword;
            cout<<"Password updated successfully."<<std::flush;
            timer(2);
            return;
        }

        void removePassword(string app_name, string user_name){
            int app_idx = app_name_exist(app_name);
            if(app_idx == -1){
                cout<<"App name not found!";
                timer(2);
                return;
            }

            auto user_data = apps[app_idx].user_data.find(user_name);
            
            if(user_data == apps[app_idx].user_data.end()){
                clearScreen();
                cout<<"Username does not exists!"<<std::flush;
                timer(2);
                return;
            }

            apps[app_idx].user_data.erase(user_name);
            cout<<"Password removed successfully."<<std::flush;
            timer(2);
            return;
        }

        void displayPasswords() {
            // First, create a sorted structure by app name
            map<string, map<string, string>> sortedApps;
            
            for (auto& app : apps) {
                sortedApps[app.second.app_name] = app.second.user_data;
            }
            
            // Display in a tree structure
            cout << "Password Manager Tree:" << endl;
            cout << "======================" << endl << endl;
            
            auto appIt = sortedApps.begin();
            while (appIt != sortedApps.end()) {
                const auto& app = *appIt;
                bool isLastApp = (++appIt == sortedApps.end());
                
                cout << (isLastApp ? "└── " : "├── ") << app.first << endl;
                
                auto userIt = app.second.begin();
                while (userIt != app.second.end()) {
                    const auto& userData = *userIt;
                    bool isLastUser = (++userIt == app.second.end());
                    
                    cout << (isLastApp ? "    " : "│   ") 
                    << (isLastUser ? "└── " : "├── ") 
                    << userData.first << " : " << userData.second << endl;
                }
            }

            cout << "\nPress RETURN / ENTER to continue…";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }

        void displayPassword(string app_name){
            
            clearScreen();

            int appIndex = app_name_exist(app_name);
            
            if(appIndex == -1) {
                cout << "App name not found!" << endl;
                timer(2);
                return;
            }
            
            cout << app_name << " Password List:" << endl;
            cout << "======================" << endl << endl;
                         
            cout << apps[appIndex].app_name << endl;
                
            auto userIt = apps[appIndex].user_data.begin();
            while (userIt != apps[appIndex].user_data.end()) {
                const auto& userData = *userIt;
                bool isLastUser = (++userIt == apps[appIndex].user_data.end());
                    
                cout << "   " << (isLastUser ? "└── " : "├── ") 
                << userData.first << " : " << userData.second << endl;
            }

            cout << "\nPress RETURN / ENTER to continue…";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
        }

        // Add getter for apps to allow SecurityManager to access it
        map<int, struct app>& getApps() {
            return apps;
        }
};

int main() {
    PasswordManager PwdMg;
    map<string, string> user;
    
    // Define the data file path
    string dataFilePath = "password_data.bin";
    SecurityManager secMgr(dataFilePath);
    
    bool auth = false;
    
    // Check if this is first run
    if (!secMgr.dataFileExists()) {
        cout << "First time setup. Creating admin account." << endl;
        string adminUser, adminPass;
        cout << "Enter admin username: ";
        cin >> adminUser;
        cout << "Enter admin password: ";
        cin >> adminPass;
        
        if (secMgr.initializeDataFile(adminUser, adminPass)) {
            cout << "Admin account created successfully!" << endl;
            user["name"] = adminUser;
            user["pass"] = adminPass;
        } else {
            cout << "Failed to create admin account. Exiting." << endl;
            return 1;
        }
    }
    
    // Authentication loop
    string name, pass;
    int attempts = 0;
    while(attempts < 3){
        clearScreen();
        cout<<"\nEnter Username: ";
        cin>>name;
        cout<<"Enter password for "+name+": ";
        cin>>pass;
        
        // First load user data to verify credentials
        map<string, string> tempUsers;
        map<int, struct app> tempApps;
        
        // Try to load user data with minimal validation
        secMgr.setMasterKey(pass);
        if (secMgr.loadData(tempApps, tempUsers)) {
            // Now verify if username exists and password matches
            auto userIt = tempUsers.find(name);
            if (userIt != tempUsers.end() && userIt->second == pass) {
                cout << "Login Successful";
                auth = true;
                
                // Now properly load all data
                user = tempUsers;
                PwdMg.getApps() = tempApps;
                
                clearScreen();
                break;
            } else {
                cout << "Login Failed: Invalid username or password";
                attempts++;
                this_thread::sleep_for(chrono::seconds(1));
                clearScreen();
            }
        } else {
            cout << "Login Failed: Decryption error";
            attempts++;
            this_thread::sleep_for(chrono::seconds(1));
            clearScreen();
        }
    }
    
    if (attempts >= 3) {
        cout << "Too many failed login attempts. Exiting." << endl;
        return 1;
    }
    
    if(auth){
        // Show welcome screen with security information
        clearScreen();
        cout << "\n\n";
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║                                                            ║\n";
        cout << "║                SECURE PASSWORD MANAGER                     ║\n";

        // Center the author line with emojis
        string author = "👨‍💻 Developed by Sai Dhinakar S 🚀";
        int totalWidth = 65;
        int leftPadding = (totalWidth - author.length()) / 2;
        int rightPadding = totalWidth - author.length() - leftPadding;

        cout << "║  " << string(leftPadding, ' ') << author << string(rightPadding, ' ') << "  ║\n";

        cout << "║                                                            ║\n";
        cout << "╠════════════════════════════════════════════════════════════╣\n";
        cout << "║                                                            ║\n";
        cout << "║  • All passwords are stored locally on your device         ║\n";
        cout << "║  • Data is encrypted.                                      ║\n";
        cout << "║  • No data is sent over the internet                       ║\n";
        cout << "║  • After 3 failed login attempts, access is blocked        ║\n";
        cout << "║                                                            ║\n";

        // Dynamic welcome message
        string welcome = "  Welcome, " + name;
        int padding = totalWidth - static_cast<int>(welcome.length());
        if (padding < 0) {
            welcome = welcome.substr(0, totalWidth);
            padding = 0;
        }

        cout << "║  " << welcome << string(padding-9, ' ') << "  ║\n";
        cout << "║                                                            ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        cout << "Type 'help' for available commands\n" << endl;
        
        // Show options - command line style
        string command, app_name, user_name, password;
        bool isContinue = true;
        
        do{
            cout << "> ";
            cin >> command;
            
            if(command == "help") {
                clearScreen();
                cout << "╔═══════════════════════════════════════════════════════════╗\n";
                cout << "║                   AVAILABLE COMMANDS                      ║\n";
                cout << "╠═══════════════════════════════════════════════════════════╣\n";
                cout << "║  add password <app> <user> <pass>                         ║\n";
                cout << "║     → Add a new password to the secure vault              ║\n";
                cout << "║                                                           ║\n";
                cout << "║  update password <app> <user> <new_pass>                  ║\n";
                cout << "║     → Update an existing password                         ║\n";
                cout << "║                                                           ║\n";
                cout << "║  remove password <app> <user>                             ║\n";
                cout << "║     → Remove a password from the vault                    ║\n";
                cout << "║                                                           ║\n";
                cout << "║  list                                                     ║\n";
                cout << "║     → Display all passwords in a tree structure           ║\n";
                cout << "║                                                           ║\n";
                cout << "║  show <app>                                               ║\n";
                cout << "║     → Show passwords for a specific application           ║\n";
                cout << "║                                                           ║\n";
                cout << "║  help                                                     ║\n";
                cout << "║     → Display this help message                           ║\n";
                cout << "║                                                           ║\n";
                cout << "║  exit                                                     ║\n";
                cout << "║     → Exit the program and secure all data                ║\n";
                cout << "╚═══════════════════════════════════════════════════════════╝\n";

                cout << "\n🔐 SECURITY REMINDER:\n";
                cout << "   Your passwords are securely encrypted and stored locally.\n";
                cout << "   Never share your master password.\n" << endl;

            }
            else if(command == "add") {
                string type;
                cin >> type;
                
                if(type == "password") {
                    cin >> app_name >> user_name >> password;
                    PwdMg.addPassword(app_name, user_name, password);
                    secMgr.saveData(PwdMg.getApps(), user);
                } else {
                    cout << "Unknown type. Use 'help' to see available commands." << endl;
                }
            }
            else if(command == "update") {
                string type;
                cin >> type;
                
                if(type == "password") {
                    cin >> app_name >> user_name >> password;
                    PwdMg.updatePassword(app_name, user_name, password);
                    secMgr.saveData(PwdMg.getApps(), user);
                } else {
                    cout << "Unknown type. Use 'help' to see available commands." << endl;
                }
            }
            else if(command == "remove") {
                string type;
                cin >> type;
                
                if(type == "password") {
                    cin >> app_name >> user_name;
                    PwdMg.removePassword(app_name, user_name);
                    secMgr.saveData(PwdMg.getApps(), user);
                } else {
                    cout << "Unknown type. Use 'help' to see available commands." << endl;
                }
            }
            else if(command == "list") {
                clearScreen();
                PwdMg.displayPasswords();
            }
            else if(command == "show") {
                cin >> app_name;
                PwdMg.displayPassword(app_name);
            }
            else if(command == "exit") {
                cout << "Exiting Password Manager. Goodbye!" << endl;
                isContinue = false;
                clearScreen();
            }
            else if (command == "about") {
                cout << "\n╔════════════════════════════════════════════╗\n";
                cout << "║          Secure Password Manager           ║\n";
                cout << "╠════════════════════════════════════════════╣\n";
                cout << "║  Version : 1.0.0                           ║\n";
                cout << "║  Author  : Sai Dhinakar S                  ║\n";
                cout << "║  GitHub  : github.com/SaiDhinakar          ║\n";
                cout << "╚════════════════════════════════════════════╝\n\n";
            }
            else {
                cout << "Unknown command. Type 'help' for available commands." << endl;
            }
        } while(isContinue);
    }

    return 0;
}