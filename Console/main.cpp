#include <iostream>
#include <map>
#include <iomanip>

using namespace std;

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
            }
            else{
                cout<<"The name or password of the app alreay exists";
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

        do{
            cout<<"\n1.Add Password\n2.Update Password\n3.Remove Password\n4.List all passwords\n5.Exit"<<endl;
            cout<<"Enter the choice(Ex : 1) : ";
            cin>>ch;

            switch (ch)
            {
            case 1:
                cout<<"Enter the name of the password : ";
                cin>>name;
                cout<<"Enter the password : ";
                cin>>pass;
                PwdMg.addPassword(name, pass);
                cout<<"Password saved successfully";
                system("clear");
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
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