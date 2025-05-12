#include <iostream>
#include <vector>
#include <conio.h>  // for _getch()
#include <iomanip>  // for setw
#include <string>
#include <limits>
#include <algorithm> // for sort
#include <fstream>
#include <sstream>

using namespace std;


class Account;
class Admin;
class Staff;
class RegularUser;
class AccountsFileHandler; // Forward declaration
class PetFileHandler;
class PetList;

class Account {
    private:
        string username;
        string password;
        string accountType;
    public:
        Account() {}
        Account(string username, string password, string accountType) {
            this->username = username;
            this->password = password;
            this->accountType = accountType;
        }
        virtual ~Account() {}
        string getUsername() const { return username; }
        string getPassword() const { return password; }
        string getAccountType() const { return accountType; }
        virtual void userInterface() const = 0;
    };


    class AccountsFileHandler {
        private:
            string filename;
            static AccountsFileHandler* instance;

            AccountsFileHandler(const string& filename) : filename(filename) {}
            AccountsFileHandler(AccountsFileHandler&) = delete;
            AccountsFileHandler& operator=(AccountsFileHandler&) = delete;

        public:
            static AccountsFileHandler* getInstance(const string& filename = "../txt-file-storage/users.txt") {
                if (!instance) {
                    instance = new AccountsFileHandler(filename);
                }
                return instance;
            }
            void saveAccounts(vector<Account*>& accounts);
            vector<Account*> loadAccounts();

            static void cleanup() {
                if (instance) {
                    delete instance;
                    instance = nullptr;
                }
            }

        };

        AccountsFileHandler* AccountsFileHandler::instance = nullptr;

// Register class definition
class Register {
    private:
        vector<Account*> accounts;
        AccountsFileHandler* fileHandler;
        string username;
        string password;
        string confirmPassword;
    public:
        Register() : fileHandler(AccountsFileHandler::getInstance()) {
            // Load accounts from file when Register is constructed
            accounts = fileHandler->loadAccounts();
        }

        ~Register() {
            // Save accounts to file when Register is destroyed
            fileHandler->saveAccounts(accounts);

            // Clean up allocated Account objects
            for (auto acc : accounts) {
                delete acc;
            }
        }
        void addAccount(string accountType);
        void displayRegisteredAccounts(string text, string type) const;
        vector<Account*>& getAccountDetails();
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
                cout << "(Remove once done) Test: Admin Detail LogIn: admin pass: admin123." << endl;
            }
        }

        void userInterface() const override {
            system("cls");
            cout << "a) View Pet Shelter Staffs" << endl;
            cout << "b) View Regular Users" << endl;
            cout << "c) Add a Staff Account" << endl;
            cout << "d) View All Accounts" << endl;
            cout << "e) Delete Account" << endl;
            cout << "x) Log Out" << endl;
        }

        void adminFunctions(Register& account){
            vector<Account*>& accounts = account.getAccountDetails();
            string choice;

            do {
                userInterface();

                cout << "\nChoice: ";
                cin >> choice;
                    system("cls");
                    if (choice == "a") {
                        account.displayRegisteredAccounts("Pet Shelter Staffs", "Staff");
                            system("pause");
                    } else if (choice == "b") {
                        account.displayRegisteredAccounts("Regular Users", "RegularUser");
                            system("pause");
                    } else if (choice == "c") {
                        account.addAccount("Staff");
                    } else if (choice == "d") {
                        account.displayRegisteredAccounts("All Accounts", "All");
                            system("pause");
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
                            system("pause");
                    } else if (choice == "x") {
                        cout << "Logging Out" << endl;
                            system("pause");
                            system("cls");
                            break;
                    }
                } while (choice != "x");

        }
};


class Staff : public Account {
private:
    PetList petlist();


public:
    Staff(){}
    Staff(string username, string password) : Account(username, password, "Staff") {}

    void userInterface() const override {
        cout << "a) Add a Pet" << endl;
        cout << "b) Delete a Pet" << endl;
        cout << "c) View Pet Adoption Request" << endl;
        cout << "d) View All Pets for Adoption" << endl;
        cout << "e) View Pending Pet Submissions" << endl;
        cout << "x) Log Out" << endl << endl;
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

        string getName() const { return name; }
        string getNumber() const { return number; }
        string getAddress() const { return address; }

        void userInterface() const override {
            cout << "a) Adopt a Pet" << endl;
            cout << "b) Add a Pet" << endl;      //Class PetList - addPet() Function
            cout << "c) View Pets" << endl;      //Class PetList - viewAllPets() Function
            cout << "d) View Pets You Added" << endl;
            cout << "e) View Pets You Adopted" << endl;
            cout << "f) Edit Profile" << endl;
            cout << "x) Log Out" << endl << endl;
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
};

vector<Account*> AccountsFileHandler::loadAccounts() {
    vector<Account*> accounts;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Note: " << filename << " not found. Starting fresh." << endl;
        return accounts;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, accountType, name, number, address;

        getline(ss, username, '-');
        getline(ss, password, '-');
        getline(ss, accountType, '-');

        Account* acc = nullptr;
        if (accountType == "Admin") {
            acc = new Admin(username, password);
        }
        else if (accountType == "Staff") {
            acc = new Staff(username, password);
        }
        else if (accountType == "RegularUser") {
            getline(ss, name, '-');
            getline(ss, number, '-');
            getline(ss, address, '-');
            acc = new RegularUser(username, password, accountType, name, number, address);
        }

        if (acc) accounts.push_back(acc);
    }
    file.close();
    return accounts;
}

void AccountsFileHandler::saveAccounts(vector<Account*>& accounts) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Failed to open " << filename << endl;
        return;
    }

    for (const Account* acc : accounts) {
        file << acc->getUsername() << "-"
                << acc->getPassword() << "-"
                << acc->getAccountType();

        // Handle RegularUser-specific fields
        if (const RegularUser* user = dynamic_cast<const RegularUser*>(acc)) {
            file << "-" << user->getName()
                    << "-" << user->getNumber()
                    << "-" << user->getAddress();
        }
        file << "\n";
        cout << "[DEBUG] Saved " << accounts.size() << " account(s)." << endl;

    }
    file.close();
}

// ———————— IMPLEMENTATION OF REGISTER METHODS —————————


void Register::addAccount(string accountType) {
    bool correctPassword;
    bool existingUsername;
    char ch;

    do {
        existingUsername = false;
        cout << "Register" << endl;
        cout << "Username: ";
        cin >> username;

        // Check if username already exists
        for (const auto& acc : accounts) {
            if (acc->getUsername() == username) {
                system("cls");
                cout << "Username Already Exists. Please try another.\n";
                existingUsername = true;
            }
        }
    } while (existingUsername);

    do {
        correctPassword = true;
        password = "";
        confirmPassword = "";

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

        cout << "\nRe-enter Password: ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!confirmPassword.empty()) {
                    confirmPassword.pop_back();
                    cout << "\b \b";
                }
            } else {
                confirmPassword += ch;
                cout << '*';
            }
        }

        if (password != confirmPassword) {
            correctPassword = false;
            cout << "\nPasswords do not match. Try again.\n";
        }

    } while (!correctPassword);

    Account* newAccount = nullptr;

    if (accountType == "Admin") {
        newAccount = new Admin(username, password);
    } else if (accountType == "Staff") {
        newAccount = new Staff(username, password);
    } else if (accountType == "RegularUser") {
        string name, number, address;
        cin.ignore(); // Clear newline from previous input
        system("cls");
        cout << "Personal Details:" << endl;
        cout << "Name: ";
        getline(cin, name);
        cout << "Mobile Number: ";
        getline(cin, number);
        cout << "Address: ";
        getline(cin, address);
        newAccount = new RegularUser(username, password, "RegularUser", name, number, address);
    } else {
        cout << "Invalid account type. Registration failed.\n";
        return;
    }

    accounts.push_back(newAccount);
    fileHandler->saveAccounts(accounts);
    cout << "\nAccount created successfully!" << endl;
    system("pause");
    system("cls");
}

void Register::displayRegisteredAccounts(string text, string type) const {
    cout << string(70, '-') << endl;
    cout << "\t\t\t" << text << endl;
    cout << string(70, '-') << endl;
    cout << setw(25) << left << "Username"
        << setw(30) << left << "Password"
        << setw(20) << left << "Account Type" << endl;
    cout << string(70, '-') << endl;

    if (type == "All") {
        for (const auto& account : accounts) {
            cout << setw(25) << left << account->getUsername()
                << setw(30) << left << account->getPassword()
                << setw(20) << left << account->getAccountType() << endl;
        }
    } else {
        for (const auto& account : accounts) {
            if (account->getAccountType() == type) {
                cout << setw(25) << left << account->getUsername()
                    << setw(30) << left << account->getPassword()
                    << setw(20) << left << account->getAccountType() << endl;
            }
        }
    }
    cout << endl;
}

vector<Account*>& Register::getAccountDetails() {
    return accounts;
}



class LogIn {
    private:
        string username;
        string password;

    public:
        Account* logIn(Register& account) {
            while (true) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Log In" << endl;
                cout << "Username: ";
                getline(cin, username);

                vector<Account*>& accounts = account.getAccountDetails();
                Account* foundAccount = nullptr;

                for (auto acc : accounts) {
                    if (acc->getUsername() == username) {
                        foundAccount = acc;
                        break;
                    }
                }

                if (!foundAccount) {
                    cout << "Username not found!" << endl;
                    continue; // Retry
                }

                // Masked password input
                char ch;
                password = "";
                cout << "Password: ";
                while ((ch = _getch()) != '\r') {
                    if (ch == '\b') {
                        if (!password.empty()) {
                            password.pop_back();
                            cout << "\b \b";
                        }
                    } else {
                        password += ch;
                        cout << '*';
                    }
                }

                if (password == foundAccount->getPassword()) {
                    cout << "\n\nLogin successful!" << endl;
                    system("pause");
                    return foundAccount; // ✅ Return pointer to logged-in Account
                } else {
                    cout << "\nIncorrect password!" << endl;
                    continue; // Retry
                }
            }

            return nullptr; // Fallback (shouldn’t be reached)
        }
    };
    // Should never reach here
class Pet {
private:
    string name;
    float age;
    string type;
    string status;
    string submittedBy;

public:
    Pet(string name, float age, string type, string status, string submittedBy) {
        this->name = name;
        this->age = age;
        this->type = type;
        this->status = status;
        this->submittedBy = submittedBy;
    }

    string getStatus() const {
        return status;
    }

    string getSubmittedBy() const {
        return submittedBy;
    }

    void setStatus (string status) {
        this->status = status;
    }

    string getName() const {
        return name;
    }
    float getAge() const {
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
    Dog(string name, float age, string type, string breed, string status, string submittedBy) : Pet(name, age, "Dog", status, submittedBy) {
        this->breed = breed;
    }

    string getBreed() const { return breed; }

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
        Cat(string name, float age, string type, string breed, string status, string submittedBy) : Pet(name, age, "Cat", status, submittedBy) {
            this->breed = breed;
        }

        string getBreed() const { return breed; }

        void viewPet() const override {
            cout << "Cat Name: " << getName() << endl
                << "Age: " << getAge() << endl
                << "Breed: " << breed;
        }
    };

class PetFileHandler {
private:
    string filename;
    static PetFileHandler* instance;

    // Private constructor for Singleton
    PetFileHandler(const string& filename) : filename(filename) {}

    // Prevent copying
    PetFileHandler(const PetFileHandler&) = delete;
    PetFileHandler& operator=(const PetFileHandler&) = delete;

public:
    // Singleton access
    static PetFileHandler* getInstance(const string& filename = "../txt-file-storage/pets.txt") {
        if (!instance) {
            instance = new PetFileHandler(filename);
        }
        return instance;
    }

    // Save all pets to file
    void savePets(const vector<Pet*>& pets) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Failed to open " << filename << endl;
            return;
        }

        for (const Pet* pet : pets) {
            file << pet->getName() << "-"
                    << pet->getAge() << "-"
                    << pet->getType();


            // Handle Dog-specific fields
            if (const Dog* dog = dynamic_cast<const Dog*>(pet)) {
                file << "-" << dog->getBreed();
            }
            // Handle Cat-specific fields
            else if (const Cat* cat = dynamic_cast<const Cat*>(pet)) {
                file << "-" << cat->getBreed();
            }
            file << "-" << pet->getStatus() << "-"
                    << pet->getSubmittedBy();
            file << "\n";
        }
        file.close();
    }

    // Load all pets from file
    vector<Pet*> loadPets() {
        vector<Pet*> pets;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Note: " << filename << " not found. Starting fresh." << endl;
            return pets;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, ageStr, type, breed, status, submittedBy;
            float age;

            getline(ss, name, '-');
            getline(ss, ageStr, '-');
            getline(ss, type, '-');
            getline(ss, breed, '-');
            getline(ss, status, '-');
            getline(ss, submittedBy, '-');


            try {
                age = stof(ageStr);
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid age format for pet " << name << endl;
                continue;  // Skip this record
            }

            Pet* pet = nullptr;
            if (type == "Dog") {
                pet = new Dog(name, age, type, breed, status, submittedBy);
            }
            else if (type == "Cat") {
                pet = new Cat(name, age, type, breed, status, submittedBy);
            }

            if (pet) pets.push_back(pet);
        }
        file.close();
        return pets;
    }

    // Cleanup Singleton instance
    static void cleanup() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
};

// Initialize static member
PetFileHandler* PetFileHandler::instance = nullptr;


class PetList {
    private:
        vector<Pet*> listOfPets; //List of Pets
        PetFileHandler* fileHandler;
        string getBreed(const Pet* pet) const {
            if (auto dog = dynamic_cast<const Dog*>(pet)) {
                return dog->getBreed();
            } else if (auto cat = dynamic_cast<const Cat*>(pet)) {
                return cat->getBreed();
            }
            return "Unknown";
        }
        static bool compareByAgeAscending(Pet* a, Pet* b) {
            return a->getAge() < b->getAge();
        }

        static bool compareByAgeDescending(Pet* a, Pet* b) {
            return a->getAge() > b->getAge();
        }

    vector<Pet*> getPendingPets() const {
            vector<Pet*> pending;
            for (Pet* pet : listOfPets) {
                if (pet->getStatus() == "pending") {
                    pending.push_back(pet); // Stores pointer to original
                }
            }
            return pending;
        }

    vector<Pet*> getAvailablePets() const {
            vector<Pet*> availablePets;
            for (Pet* pet : listOfPets) {
                if (pet->getStatus() == "available") {
                    availablePets.push_back(pet);
                }
            }
        }

    public:
        PetList() : fileHandler(PetFileHandler::getInstance()) {
            // Load pets from file when PetList is created
            listOfPets = fileHandler->loadPets();
        }

        ~PetList() {
            // Save pets to file when PetList is destroyed
            fileHandler->savePets(listOfPets);

            // Clean up memory
            for (auto pet : listOfPets) {
                delete pet;
            }
        }
    void viewAndApprovePets() {
            vector<Pet*> pendingPets = getPendingPets();

            if (pendingPets.empty()) {
                cout << "No pending pets for approval.\n";
                system("pause");
                return;
            }

            // Display pending pets with numbers
            cout << string(90, '-') << endl;
            cout << "\t\t\t\tPending Pets" << endl;
            cout << string(90, '-') << endl;
            cout << setw(5) << left << "No."
                 << setw(20) << left << "Name"
                 << setw(10) << left << "Age"
                 << setw(15) << left << "Type"
                 << setw(20) << left << "Breed"
                 << setw(20) << left << "Submitted By" << endl;
            cout << string(90, '-') << endl;

            for (size_t i = 0; i < pendingPets.size(); ++i) {
                Pet* pet = pendingPets[i];
                cout << setw(5) << left << i+1
                     << setw(20) << left << pet->getName()
                     << setw(10) << left << pet->getAge()
                     << setw(15) << left << pet->getType()
                     << setw(20) << left << getBreed(pet)
                     << setw(20) << left << pet->getSubmittedBy() << endl;
            }
            cout << string(90, '-') << endl << endl;

            cout << "Enter pet number to approve (0 to cancel): ";
            int choice;
            cin >> choice;

            if (choice > 0 && static_cast<size_t>(choice) <= pendingPets.size()) {
                // Directly modify the original pet
                pendingPets[choice-1]->setStatus("available");
                fileHandler->savePets(listOfPets); // Save changes
                cout << "\nApproved: " << pendingPets[choice-1]->getName() << endl;
            } else if (choice != 0) {
                cout << "Invalid selection.\n";
            }

            system("pause");
        }

        void addPet(const Account* account) {
            string accountType = account->getAccountType();
            cout << "Please Fill Out The Details:" << endl;
            cout << "Add A Pet:" << endl;

            string name, type, breed;
            float age;

            cout << "Name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Age: ";
            cin >> age;

            cout << "Type (A = Dog, B = Cat): ";
            cin.ignore();
            getline(cin, type);

            string status;
            string submittedBy;
            if (accountType == "RegularUser") {
                status = "pending";
                submittedBy = account->getUsername();
            }else {
                status = "available";
                submittedBy = "Staff";
            }

            Pet* newPet = nullptr;

            if (type == "A") {
                cout << "Dog's Breed: ";
                getline(cin, breed);
                newPet = new Dog(name, age, "Dog", breed, status, submittedBy);
            } else if (type == "B") {
                cout << "Cat's Breed: ";
                getline(cin, breed);
                newPet = new Cat(name, age, "Cat", breed, status, submittedBy);
            } else {
                cout << "Invalid pet type. Please enter A or B." << endl;
                return;
            }

            listOfPets.push_back(newPet);
            fileHandler->savePets(listOfPets);
            system("cls");
            cout << "Pet added: " << endl;
            newPet->viewPet();
            cout << endl << endl;
            system ("pause");
            system ("cls");
        }

        void deletePet() {
            string name;
            cout << "Deleting a Pet From The List" << endl;
            cout << "Please enter the Name of the Pet: ";
                getline(cin, name);
            for (auto pet = listOfPets.begin(); pet != listOfPets.end(); ++pet) {
                if ((*pet)->getName() == name) {
                    delete *pet;
                    listOfPets.erase(pet);
                    fileHandler->savePets(listOfPets);
                    cout << "Deleted Pet: " << name << endl;
                    return;
                }
            }
            cout << "Pet not found: " << name << endl;
        }

        void viewAllPets(string type) const {
            vector<Pet*> availablePets = getAvailablePets();

            system("cls");
            if (availablePets.empty()) {
                cout << "No pets available." << endl << endl;
                system("pause");
                system("cls");
                return;
            }

            if (type == "All") {
                cout << string(80, '-') << endl;
                cout << "\t\t\tAvailable Pets" << endl;
                cout << string(80, '-') << endl;
                cout << setw(25) << left << "Name"
                    << setw(25) << left << "Age"
                    << setw(25) << left << "Type"
                    << setw(25) << left << "Breed" << endl;
                cout << string(80, '-') << endl;
                    for (const auto pet : availablePets) {
                        cout << setw(25) << left << pet->getName()
                            << setw(25) << left << pet->getAge()
                            << setw(25) << left << pet->getType()
                            << setw(25) << left << getBreed(pet) << endl;
                    }
            } else if (type == "Dog") {
                cout << string(80, '-') << endl;
                cout << "\t\t\tAvailable Dogs" << endl;
                cout << string(80, '-') << endl;
                cout << setw(25) << left << "Name"
                    << setw(25) << left << "Age"
                    << setw(25) << left << "Breed" << endl;
                cout << string(80, '-') << endl;
                    for (const auto pet : availablePets) {
                        if (pet->getType() == "Dog") {
                        cout << setw(25) << left << pet->getName()
                            << setw(25) << left << pet->getAge()
                            << setw(25) << left << getBreed(pet) << endl;
                        }
                    }
            } else if (type == "Cat") {
                cout << string(80, '-') << endl;
                cout << "\t\t\tAvailable Cats" << endl;
                cout << string(80, '-') << endl;
                cout << setw(25) << left << "Name"
                    << setw(25) << left << "Age"
                    << setw(25) << left << "Breed" << endl;
                cout << string(80, '-') << endl;
                    for (const auto pet : availablePets) {
                        if (pet->getType() == "Cat") {
                        cout << setw(25) << left << pet->getName()
                            << setw(25) << left << pet->getAge()
                            << setw(25) << left << getBreed(pet) << endl;
                        }
                    }
            } else {
                cout << "Type of Pet Invalid. Please Try Again.";
                return;
            }
            cout << string(80, '-') << endl << endl;
            system ("pause");
            system ("cls");
        }

        void viewPetsSortedByAge(bool ascending = true) const {
            vector<Pet*> sortedPets = listOfPets;

            if (ascending) {
                sort(sortedPets.begin(), sortedPets.end(), compareByAgeAscending);
            } else {
                sort(sortedPets.begin(), sortedPets.end(), compareByAgeDescending);
            }

            if (sortedPets.empty()) {
                cout << "No pets available." << endl;
                return;
            }

            // Table Header
            cout << string(80, '-') << endl;
            cout << "\t\t\tPets Sorted by Age" << endl;
            cout << string(80, '-') << endl;
            cout << setw(25) << left << "Name"
                << setw(25) << left << "Age"
                << setw(25) << left << "Type"
                << setw(25) << left << "Breed" << endl;
            cout << string(70, '-') << endl;

            // Table Rows
            for (const auto& pet : sortedPets) {
                cout << setw(25) << left << pet->getName()
                    << setw(25) << left << pet->getAge()
                    << setw(25) << left << pet->getType()
                    << setw(25) << left << getBreed(pet) << endl;
            }

            cout << string(80, '-') << endl << endl;
        }

        void filterPet() const {
            bool validFilter;
            string choice;

            do {
                validFilter = false;
                viewAllPets("All");
                cout << "Filter Pets (Input x to return to menu):" << endl;
                cout << "a) By Type" << endl;
                cout << "b) By Age" << endl;
                cout << "\nChoice: ";
                cin >> choice;
                cin.ignore();

                if (choice == "a") {
                    validFilter = true;
                    system("cls");
                    viewAllPets("All");

                    string type;
                    bool validType;
                    do {
                        validType = false;
                        cout << "Choose a Type (Input x to return to menu):" << endl;  //Filter the table in accordance to their type
                        cout << "a) Dogs" << endl;
                        cout << "b) Cats" << endl;
                        cout << "Choice: ";
                        cin >> type;
                        cin.ignore();

                        if (type == "a") {
                            viewAllPets("Dog");
                            validType = true;
                        } else if (type == "b") {
                            viewAllPets("Cat");
                            validType = true;
                        } else if (type == "x") {
                            validType = true;
                            system("cls");
                            break;
                        } else {
                            cout << "Please enter a valid choice." << endl;
                        }
                    } while (!validType);
                } else if (choice == "b") {
                    validFilter = true;
                    system("cls");
                    viewAllPets("All");

                    string ageOrder;
                    bool validOrder;
                    do {
                        validOrder = false;
                        cout << "Sort by Age (Input x to return to menu):" << endl;           //Sort the table in accordance to their age
                        cout << "a) Ascending" << endl;
                        cout << "b) Descending" << endl;
                        cout << "Choice: ";
                        cin >> ageOrder; // Read single character
                        cin.ignore();    // Clear newline from buffer

                        if (ageOrder == "a") {
                            viewPetsSortedByAge(true); // Ascending
                            validOrder = true;
                        } else if (ageOrder == "b") {
                            viewPetsSortedByAge(false); // Descending
                            validOrder = true;
                        } else if (ageOrder == "x") {
                            validOrder = true;
                            system("cls");
                            break;
                        } else {
                            cout << "Please enter a valid choice." << endl;
                        }
                    } while (!validOrder);
                } else if (choice == "x") {
                    validFilter = true;
                    break;
                } else {
                    cout << "Please enter a valid choice." << endl;
                }
            } while (!validFilter);
        }
    void viewAndApproveAdoptions() {

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
    Staff staffAccount;
    RegularUser userAccount;
    adminAccount.addAdmin(registerAccount);

    AccountsFileHandler *filehandler = AccountsFileHandler::getInstance();
    vector<Account*> accounts = filehandler->loadAccounts();

    PetList animal;

    string choice;

        do {
            Account *currentAccount = nullptr;
            cout << "Pet Adoption System" << endl;
            cout << "a) Register" << endl;
            cout << "b) Login" << endl;
            cout << "c) Exit" << endl << endl;
            cout << "Enter choice: ";
            cin >> choice;

            system("cls");
            if (choice == "a") {
                registerAccount.addAccount("RegularUser");
            } else if (choice == "b") {
                currentAccount = logInAccount.logIn(registerAccount);;
                    system("cls");
                        if (currentAccount->getAccountType() == "Admin") {
                            adminAccount.adminFunctions(registerAccount);
                        } else if (currentAccount->getAccountType() == "Staff") {
                            do {
                                staffAccount.userInterface();
                                cout << "Choice: ";
                                cin >> choice;
                                    system("cls");
                                    if (choice == "a") {
                                        animal.addPet(currentAccount);
                                    } else if (choice == "b") {
                                        animal.deletePet();                   // LETTER C = add view pet adoption request - amaya
                                    } else if (choice == "c") {
                                        animal.viewAndApproveAdoptions();
                                    }else if (choice == "d") {
                                        animal.viewAllPets("All");
                                    }else if (choice == "e") {
                                        animal.viewAndApprovePets();
                                    } else if (choice == "x") {
                                        cout << "Logging Out" << endl;
                                        system("pause");
                                        system("cls");
                                        break;
                                    }
                            } while (choice != "x");
                        } else if (currentAccount->getAccountType() == "RegularUser") {
                            do {
                                userAccount.userInterface();
                                    cout << "\nChoice: ";
                                    cin >> choice;
                                        system("cls");
                                        if (choice == "a") {
                                            //AMAYA
                                        } else if (choice == "b") {
                                            animal.addPet(currentAccount);     //amaya ADD STATUS OF THE PET, CONDITION (IF USERTYPE == REGULARUSER, NEED NG APPROVAL FROM Staff)
                                        } else if (choice == "c") {
                                            animal.filterPet();
                                        } else if (choice == "d") {
                                            //WALA PA ME NAGAGAWA NA FUNCTION FOR VIEWING PETS NA INADD NI REGULAR USER MISMO
                                        } else if (choice == "e") {
                                            //WALA PA RIN NAGAGAWA NA FUNCTION FOR VIEWING PETS NA INADOPT NI USER
                                        } else if (choice == "f") {
                                            userAccount.editPersonalInformation(registerAccount);   //Need iretrieve ung info ng current session
                                        } else if (choice == "x") {
                                            cout << "Logging Out" << endl;
                                            system("pause");
                                            system("cls");
                                            break;
                                        }
                            } while (choice != "x");
                        }
            } else if (choice == "c") {
                cout << "Exiting Program";
            } else {
                cout << "Invalid choice. Try again.\n";
            }

        } while (choice != "c");



        return 0;
    }