#include <iostream>
#include <vector>
#include <conio.h>
#include <iomanip>
using namespace std;

class Account{
    private:
        string username;
        string password;
        string accountType;
    public:
        Account(string username, string password, string accountType){
            this->username = username;
            this->password = password;
            this->accountType = accountType;
        }
        string getUsername() const {
            return username;
        }
        string getPassword() const {
            return password;
        }
        string getAccountType() const {
            return accountType;
        }
};

class Register{
    private:
        vector <Account> accounts;
        string username;
        string password;
        string confirmPassword;
    public:
        void addAccount(string accountType){
            bool correctPassword;
            char ch;
                cout << "Register" << endl;
                cout << "Username: ";
                    cin >> username;
                        for (auto& account : accounts){ 
                            if (account.getUsername() == username) {  //If the username already exists in the system's database, it would promt the user to input anotheru sername
                                cout << "Username Already Exists. Please try another.";
                            }
                        }

                    do {
                        correctPassword = true;
                        cout << "\nPassword: ";
                        while ((ch = _getch()) != '\r') { // Stops when Enter is pressed
                            if (ch == '\b') { // Backspace key
                                if (!password.empty()) {
                                    password.pop_back();  // Remove last character
                                    cout << "\b \b"; // Erase '*' from console
                                }
                            } else {
                                password += ch;
                                cout << '*';  // Mask input with '*'
                            }
                        }
                    
                        cout << "\nRe-enter Password: ";                //Re-enter password to verify if the user correctly entered the desired password
                                while ((ch = _getch()) != '\r') { // Stops when Enter is pressed
                                    if (ch == '\b') { // Backspace key
                                        if (!confirmPassword.empty()) {
                                            confirmPassword.pop_back();  // Remove last character
                                            cout << "\b \b"; // Erase '*' from console
                                        }
                                    } else {
                                        confirmPassword += ch;
                                        cout << '*';  // Mask input with '*'
                                    }
                                }
                                
                                    if (password != confirmPassword){     //If the re-entered password does not match the initial password input, it would set the flag as false
                                        correctPassword = false;         //It would go back to the process of inputting the password until they are able to correctly re-enter the password;
                                        password = "";               //Ensures that the password is reseted to ensure a loop does not exist due to the comparison between the old input and the new input
                                        confirmPassword = "";
                                    }
                        } while (!correctPassword); 
                accounts.emplace_back(username, password, accountType);    //Store the account in the vector
            }

        void displayRegisteredAccounts(string text, string type) const {
                cout << string(70, '-') << endl;
                cout << "\t\t\ttext" << endl;
                cout << string(70, '-') << endl;
                cout << setw(25) << left << "Username"
                    << setw(30) << left << "Password"
                    << setw(20) << left << "Account Type" << endl;
                cout << string(70, '-') << endl;
        
                for (const auto& account : accounts) {                         //Display Account for regular users and pet shelter staffs
                    if (account.getAccountType() == type) {
                        cout << setw(25) << left << account.getUsername()
                            << setw(30) << left << account.getPassword()
                            << setw(20) << left << account.getAccountType() << endl;
                    }
                }

                if (type == "All") {                                      //Display all the accounts
                    for (const auto& account : accounts) {
                            cout << setw(25) << left << account.getUsername()
                                << setw(30) << left << account.getPassword()
                                << setw(20) << left << account.getAccountType() << endl;
                    }
                }
            }

        vector<Account>& getAccountDetails() {
            return accounts;
        }
};

class LogIn{
    private:
        string username;
        string password;
    public:
        void logIn(Register& account){
            bool validLogIn;
            vector<Account>& accounts = account.getAccountDetails();

            do {
            cout << "Log In" << endl;
                cout << "Username: ";
                    cin >> username;
                for (const auto& acc : accounts) {
                    char ch;
                    if (username == acc.getUsername()) {  // Username found
                        cout << "Password: ";
                        while ((ch = _getch()) != '\r') { // Stops when Enter is pressed
                            if (ch == '\b') { // Backspace key
                                if (!password.empty()) {
                                    password.pop_back();  // Remove last character
                                    cout << "\b \b"; // Erase '*' from console
                                }
                            } else {
                                password += ch;
                                cout << '*';  // Mask input with '*'
                            }
                        }

                        if (password == acc.getPassword()) {  // Password matches
                            cout << "\nLogin successful!" << endl;
                                return;
                        } else {
                            cout << "\nIncorrect password!" << endl;
                            validLogIn = false;
                                continue;
                        }
                    } else {
                        cout << "Username not found!" << endl;
                    }
                }
            } while (!validLogIn);
        }
};

class Admin : public Account {
    private:
        string usernameToDelete;
    public:
        Admin(string username, string password) : Account(username, password, "Admin") {}
    
        void manageUsers(Register& account){
            vector<Account>& accounts = account.getAccountDetails();
            string choice;
            cout << "a) View Pet Shelter Staffs" << endl;
            cout << "b) View Regular Users" << endl;
            cout << "c) Add an Account" << endl;
            cout << "d) View All Accounts" << endl;
            cout << "e) Delete Account" << endl;

            cout << "Choice: " << endl;
            cin >> choice;
                if (choice == "a") {
                    account.displayRegisteredAccounts("Pet Shelter Staffs", "PetShelter");
                } else if (choice == "b") {
                    account.displayRegisteredAccounts("Regular Users", "RegularUser");                    
                } else if (choice == "c") {
                    account.addAccount("PetShelter");
                } else if (choice == "d") {
                    account.displayRegisteredAccounts("All Accounts", "All");
                } else if (choice == "e") {
                    account.displayRegisteredAccounts("All Accounts", "All");
                    cout << "\nEnter the username of the account to delete: ";
                    cin >> usernameToDelete;

                    for (size_t i = 0; i < accounts.size(); ++i) {
                        if (accounts[i].getUsername() == usernameToDelete) {
                            accounts.erase(accounts.begin() + i); // Remove element at index i
                            --i; // Recheck once the delete is done
                        }
                    }
    
                    cout << "Account: " << usernameToDelete << "' has been deleted." << endl;
                }
    
                }
};


class PetShelter : public Account {
    public:
        PetShelter(string username, string password) : Account(username, password, "PetShelter") {}
};

class RegularUser : public Account {
    public:
        RegularUser(string username, string password) : Account(username, password, "RegularUser") {}
};

int main(){

    Register account;
    LogIn user;
    Admin adminUser("admin", "admin");

    account.addAccount("RegularUser");
    system("cls");
    user.logIn(account);

    return 0;
}