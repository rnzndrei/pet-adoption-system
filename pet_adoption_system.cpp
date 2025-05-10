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
        Account(){}
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
        virtual void userInterface() const = 0; 
};


class Register{
    private:
        vector <Account*> accounts;
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

            // Check if username already exists
                for (const auto& acc : accounts) {
                    if (acc->getUsername() == username) {
                        cout << "Username Already Exists. Please try another.\n";
                        return; // Exit if username is already taken
                    }
                }

            do {
                correctPassword = true;
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

                cout << "Re-enter Password: "; // Re-enter password to verify
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

                    if (password != confirmPassword) {
                        correctPassword = false;
                        password = ""; // Reset password
                        confirmPassword = "";
                        cout << "Passwords do not match. Try again.\n";
                    }
            } while (!correctPassword);

            Account* newAccount = nullptr;

                if (accountType == "Admin") {
                    newAccount = new Admin(username, password);
                } else if (accountType == "PetShelter") {
                    newAccount = new PetShelter(username, password);
                } else if (accountType == "RegularUser") {
                    string name, number, address;
                    cout << "Name: ";
                        getline(cin, name);
                    cout << "Mobile Number: ";
                        getline(cin, number);
                    cout << "Address: ";
                        getline(cin, address);
                    newAccount = new RegularUser(username, password, accountType, name, number, address);
                } else {
                    cout << "Invalid account type. Registration failed.\n";
                    return;
                }

            // Add the new account to the list
            accounts.push_back(newAccount);
            cout << "Account created successfully!" << endl;
            system("pause");
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
                    if (account->getAccountType() == type) {
                        cout << setw(25) << left << account->getUsername()
                            << setw(30) << left << account->getPassword()
                            << setw(20) << left << account->getAccountType() << endl;
                    }
                }

                if (type == "All") {                                      //Display all the accounts
                    for (const auto& account : accounts) {
                            cout << setw(25) << left << account->getUsername()
                                << setw(30) << left << account->getPassword()
                                << setw(20) << left << account->getAccountType() << endl;
                    }
                }
            }

        vector<Account*>& getAccountDetails() {
            return accounts;
        }
};

class Admin : public Account {
    private:
        string usernameToDelete;
    public:
        Admin(){}
        Admin(string username, string password) : Account(username, password, "Admin") {}
    
        void addAdmin(Register& account){
            vector<Account*>& accounts = account.getAccountDetails();
            // Check if an Admin already exists
            bool adminExists = false;
            for (const auto& acc : accounts) {
                if (acc->getAccountType() == "Admin") {
                    adminExists = true;
                    break;
                }
            }
        
            // If not, create one
            if (!adminExists) {
                accounts.push_back(new Admin("admin", "admin123"));
                cout << "Admin Detail LogIn: admin", "admin123.\n";
            }
        }

        void manageUsers(Register& account){
            vector<Account*>& accounts = account.getAccountDetails();
            string choice;

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
                        if (accounts[i]->getUsername() == usernameToDelete) {
                            accounts.erase(accounts.begin() + i); // Remove element at index i
                            --i; // Recheck once the delete is done
                        }
                    }
    
                    cout << "Account: " << usernameToDelete << "' has been deleted." << endl;
                }
                
        }
        
        void userInterface() const override {
            cout << "a) View Pet Shelter Staffs" << endl;   
            cout << "b) View Regular Users" << endl;
            cout << "c) Add an Account" << endl;
            cout << "d) View All Accounts" << endl;
            cout << "e) Delete Account" << endl;
        }

};


class PetShelter : public Account {
    public:
        PetShelter(){}
        PetShelter(string username, string password) : Account(username, password, "PetShelter") {}

        void userInterface() const override {
            cout << "a) Add a Pet" << endl;
            cout << "b) Delete a Pet" << endl;
            cout << "c) View Pet Adoption Request" << endl;
        }
};

class RegularUser : public Account {
    private:
        string name;
        string number;
        string address;
    public:
        RegularUser(){}
        RegularUser(string username, string password, string type, string name, string number, string address) : Account(username, password, "RegularUser") {
            this->name = name;
            this->number = number;
            this->address = address; 
        }
        
        void inputPersonalInformation(Register& account) {
            vector<Account*>& accounts = account.getAccountDetails();
            
            cout << "Name: ";
                getline(cin, name);
            cout << "Mobile Number: ";
                getline(cin, number);
            cout << "Address: ";
                getline(cin, address);    
            accounts.push_back(new RegularUser(getUsername(), getPassword(), getAccountType(), name, number, address));
        }

        void editPersonalInformation(Register& account) {
            vector<Account*>& accounts = account.getAccountDetails();

            string newName = this->name;
            string newNumber = this->number;
            string newAddress = this->address;

            cout << "\nEdit Personal Information" << endl;

            cout << "Current Name: " << newName << "\nEnter new name (or press Enter to keep): ";
            cin.ignore();
            getline(cin, newName);
            cout << "Current Mobile Number: " << newNumber << "\nEnter new mobile number (or press Enter to keep): ";
            getline(cin, newNumber);
            cout << "Current Address: " << newAddress << "\nEnter new address (or press Enter to keep): ";
            getline(cin, newAddress);

            //If the user inputted something, procced to change the name. Otherwise, if they inputted empty or pressed enter
            //it would keep the current details
            this->name = newName.empty() ? this->name : newName;
            this->number = newNumber.empty() ? this->number : newNumber;
            this->address = newAddress.empty() ? this->address : newAddress;

            //Update the information in the account list
            for (size_t i = 0; i < accounts.size(); ++i) {
                if (accounts[i]->getUsername() == this->getUsername()) {
                    delete accounts[i]; // Delete previous details

                    // Replace with the newly inputted information details
                    accounts[i] = new RegularUser(
                        this->getUsername(),
                        this->getPassword(),
                        this->getAccountType(),
                        this->name,
                        this->number,
                        this->address
                    );

                    cout << "Profile updated successfully!" << endl;
                    break;
                }
            }
        }

        void userInterface() const override {
            cout << "a) Adopt a Pet" << endl;
            cout << "b) Add a Pet" << endl;      //Class PetList - addPet() Function
            cout << "c) View Pets" << endl;      //Class PetList - viewAllPets() Function
            cout << "d) View Pets You Added" << endl;      
            cout << "e) View Pets You Adopted" << endl;            
            cout << "f) Edit Profile" << endl;
        }
};
class LogIn{
    private:
        string username;
        string password;
    public:
        string logIn(Register& account){
            bool validLogIn;
            vector<Account*>& accounts = account.getAccountDetails();

            do {
            cout << "Log In" << endl;
                cout << "Username: ";
                    cin >> username;
                for (const auto& acc : accounts) {
                    char ch;
                    if (username == acc->getUsername()) {  // Username found
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

                        if (password == acc->getPassword()) {  // Password matches
                            cout << "\nLogin successful!" << endl;
                                return acc->getAccountType();
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
            
            return "";
        }
};
class Pet {
private:
    string name;
    string age;
    string type;

public:
    Pet(string name, string age, string type) {
        this->name = name;
        this->age = age;
        this->type = type;
    }

    string getName() const {
        return name;
    }
    string getAge() const {
        return age;
    }
    string getType() const {
        return type;
    }
    virtual void viewPet() const = 0;
};

class Dog : public Pet {
private:
    string breed;

public:
    Dog(string name, string age, string type, string breed) : Pet(name, age, "Dog") {
        this->breed = breed;
    }

    void viewPet() const override {
        cout << "Dog Name: " << getName() << endl
             << "Age: " << getAge() << endl
             << "Breed: " << breed;
    }
};

class Cat : public Pet {
    private:
        string breed;
    public:
        Cat(string name, string age, string type, string breed) : Pet(name, age, "Cat") {}
    
        void viewPet() const override {
            cout << "Cat Name: " << getName() << endl
                << "Age: " << getAge() << endl
                << "Breed: " << breed;
        }
    };
        
class PetList {
private:
    vector<Pet*> listOfPets; //List of Pets
    string name;
    string age;
    string type;
    string breed;
public:
    void addPet(Pet* pet) {
        cout << "Please Fill Out The Details:" << endl;
        cout << "Add A Pet:" << endl;
        cout << "Name: ";
            getline(cin, name);
        cout << "Age: ";
            getline(cin, age);
        cout << "Type (A = Dog, B = Cat): ";
            getline(cin, type);
            
            Pet* newPet = nullptr;
        
            if (type == "A") {
                cout << "Dog's Breed: ";
                getline(cin, breed);
                newPet = new Dog(name, age, "Dog", breed);
            } else if (type == "B") {
                cout << "Cat's Breed: ";
                getline(cin, breed);
                newPet = new Cat(name, age, "Cat", breed);
            } else {
                cout << "Invalid pet type. Please enter A or B." << endl;
                return;
            }
    
        listOfPets.push_back(pet);
        cout << "Pet added: " << endl;
        pet->viewPet();
    }

    void deletePet() {
        cout << "Deleting a Pet From The List" << endl;
        cout << "Please enter the Name of the Pet: ";
            getline(cin, name);
        for (auto pet = listOfPets.begin(); pet != listOfPets.end(); ++pet) {
            if ((*pet)->getName() == name) {
                delete *pet;
                listOfPets.erase(pet);
                cout << "Deleted Pet: " << name << endl;
                return;
            }
        }
        cout << "Pet not found: " << name << endl;
    }

    void viewAllPets() const {
        if (listOfPets.empty()) {
            cout << "No pets available." << endl;
            return;
        }
        for (const auto pet : listOfPets) {
            pet->viewPet();
        }
    }
};



int main(){

    /* Functions to do:
    Register /
    Log In /
    Log Out - Amaya
    text file of accounts and pets - Amaya
    Validations - Lyrus

    Admin
    - manage users /
    - add pet shelter staffs /
    - view users /
    - generation of reports - amaya
    Pet Shelters
    - add a pet /
    - delete a pet /
    - confirm pet adoption requests - amaya
    Regular users
    - view pets (type, age maturity) 
    - adding a pet for adoption 
    - adopt a pet
    - view adopted pets
    - view pets your registered
    */

    Register registerAccount;
    LogIn logInAccount;
    //Users
    Admin adminAccount;
    PetShelter staffAccount;
    RegularUser userAccount;
    adminAccount.addAdmin(registerAccount);

    int choice;

        do {
            cout << "Pet Adoption System" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "3. Exit" << endl << endl;
            cout << "Enter choice: ";
            cin >> choice;
    
            if (choice == 1) {
                registerAccount.addAccount("RegularUser");
            } else if (choice == 2) {
                string userType = logInAccount.logIn(registerAccount);
    
                if (userType == "Admin") {
                    adminAccount.userInterface();
                    adminAccount.manageUsers(registerAccount);
                } else if (userType == "PetShelter") {
                    staffAccount.userInterface();
                } else if (userType == "RegularUser") {
                    userAccount.userInterface();
                }
            } else if (choice == 3) {
                cout << "Exiting Program";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
    
        } while (choice != 3);
    
        return 0;
    }