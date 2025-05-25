#include <iostream>
#include <map>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;

void timer(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
    system("clear");
}

struct app{
    string app_name;
    map<string, string> user_data;
}app;

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
                system("clear");
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
                system("clear");
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

            cout << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            for(int i = 15; i > 0; i--) {
                cout << "\rThis screen will clear in " << i << " seconds... " << std::flush;
                for(int j = 0; j < 10; j++) {
                    this_thread::sleep_for(chrono::milliseconds(100));
                }
            }
            system("clear");
        }
};

int main(){
    PasswordManager PwdMg;
    map<string, string>user;

    int ch;
    bool auth = false;

    user["name"] = "test";
    user["pass"] = "test@123";

    string name, pass;
    while(true){
        system("clear");
        cout<<"\nEnter Username : ";
        cin>>name;
        cout<<"Enter password for "+name+" : ";
        cin>>pass;

        if(user["name"] == name && user["pass"] == pass){
            cout<<"login Successfull";
            auth = true;
            system("clear");
            break;
        }
        else{
            cout<<"Login Failed";
            system("clear");
        }
    }
    
    if(auth){
        // Show options 
        string app_name, user_name, password;

        do{
            cout<<"\n1.Add Password\n2.Update Password\n3.Remove Password\n4.List all passwords\n5.Exit"<<endl;
            cout<<"Enter the choice(Ex : 1) : ";
            cin>>ch;
            system("clear");
            switch (ch) {
                case 1:
                    cout<<"Enter the app name : ";
                    cin>>app_name;
                    cout<<"Enter the username : ";
                    cin>>user_name;
                    cout<<"Enter the password : ";
                    cin>>password;
                    PwdMg.addPassword(app_name, user_name, password);
                    break;
                
                case 2:
                    cout<<"Enter the app name : ";
                    cin>>app_name;
                    cout<<"Enter the username : ";
                    cin>>user_name;
                    cout<<"Enter the password : ";
                    cin>>password;
                    PwdMg.updatePassword(app_name, user_name, password);
                    break;
                
                case 3:
                    cout<<"Enter the app name : ";
                    cin>>app_name;
                    cout<<"Enter the username : ";
                    cin>>user_name;
                    PwdMg.removePassword(app_name, user_name);
                    break;

                case 4:
                    system("clear");
                    PwdMg.displayPasswords();
                    break;

                case 5:
                    break;
                    system("clear");
                    
                default:
                    cout<<"Invalid Choice";
                    system("clear");
            }
        }while(ch > 0 && ch < 5);
    }

    return 0;
}