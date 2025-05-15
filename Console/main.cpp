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

class PasswordManager {
    
    map<string, string>passwords;
    
    bool find_matching(string name, string pass){
        for(auto password : passwords){
            if(password.first == name || password.second == pass){
                return true;
            }
        }
        return false;
    }

    public:
        void addPassword(string name, string password){
            if(!find_matching(name, password)){
                passwords[name] = password;
                cout<<"Password addded successfully"<<std::flush;
                timer(2);
            }
            else{
                system("clear");
                cout<<"The name or password of the app alreay exists"<<std::flush;
                timer(2);
            }
        }

        void updatePassword(string name, string password, string newPassword){
            if(find_matching(name, password)){
                passwords[name] = newPassword;
                cout<<"Password updated successfully..."<<std::flush;
                timer(2);
            }
            else{
                system("clear");
                cout<<"The name or password of the app not exists"<<std::flush;
                timer(2);
            }
        }

        void removePassword(string name, string password){
            if(find_matching(name, password)){
                passwords.erase(name);
                cout<<"Password Removed successfully..."<<std::flush;
                timer(2);
            }
            else{
                system("clear");
                cout<<"The name or password of the app not exists"<<std::flush;
                timer(2);
            }
        }

        void displayPasswords(){
            
            const int nameWidth = 20;
            const int passwordWidth = 20;
            cout << left << setw(nameWidth) << "Name" << setw(passwordWidth) << "Password" << endl;
            cout << string(nameWidth + passwordWidth, '-') << endl;
            
            for(auto password : passwords){
                cout << left << setw(nameWidth) << password.first << setw(passwordWidth) << password.second << endl;
            }
            cout << "\nThis screen will clear in 15 seconds...\n"<<std::flush;
            
            timer(15);
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
        string newPass;

        do{
            cout<<"\n1.Add Password\n2.Update Password\n3.Remove Password\n4.List all passwords\n5.Exit"<<endl;
            cout<<"Enter the choice(Ex : 1) : ";
            cin>>ch;
            system("clear");
            switch (ch)
            {
            case 1:
                cout<<"Enter the name of the password : ";
                cin>>name;
                cout<<"Enter the password : ";
                cin>>pass;
                PwdMg.addPassword(name, pass);
                break;
            case 2:
                cout<<"Enter the name of the password : ";
                cin>>name;
                cout<<"Enter the password : ";
                cin>>pass;
                cout<<"Enter the new password : ";
                cin>>newPass;
                PwdMg.updatePassword(name, pass, newPass);
                break;
            case 3:
                cout<<"Enter the name of the password : ";
                cin>>name;
                cout<<"Enter the password : ";
                cin>>pass;
                PwdMg.removePassword(name, pass);
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