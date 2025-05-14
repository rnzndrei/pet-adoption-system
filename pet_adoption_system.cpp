#include <iostream>
#include <vector>
#include <conio.h>  // for _getch()
#include <iomanip>  // for setw
#include <limits>
#include <string>
#include <algorithm> // for sort
#include <fstream>   // for file handling
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
    Account() {
    }

    Account(string username, string password, string accountType) {
        this->username = username;
        this->password = password;
        this->accountType = accountType;
    }

    virtual ~Account() {
    }

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getAccountType() const { return accountType; }

    virtual void userInterface() const = 0;
};


class AccountsFileHandler {
private:
    string filename;
    static AccountsFileHandler *instance;

    AccountsFileHandler(const string &filename) : filename(filename) {
    }

    AccountsFileHandler(AccountsFileHandler &) = delete;

    AccountsFileHandler &operator=(AccountsFileHandler &) = delete;

public:
    static AccountsFileHandler *getInstance(const string &filename = "txt-file-storage/users.txt") {
        if (!instance) {
            instance = new AccountsFileHandler(filename);
        }
        return instance;
    }

    void saveAccounts(vector<Account *> &accounts);

    vector<Account *> loadAccounts();

    static void cleanup() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }
};

AccountsFileHandler *AccountsFileHandler::instance = nullptr;

// Register class definition
class Register {
private:
    vector<Account *> accounts;
    AccountsFileHandler *fileHandler;
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
        for (auto acc: accounts) {
            delete acc;
        }
    }

    void addAccount(string accountType);

    void displayRegisteredAccounts(string text, string type) const;

    vector<Account *> &getAccountDetails();
};

class Admin : public Account {
private:
    string usernameToDelete;

public:
    Admin() {
    }

    Admin(string username, string password) : Account(username, password, "Admin") {
    }

    void addAdmin(Register &account) {
        vector<Account *> &accounts = account.getAccountDetails();
        // Check if an Admin already exists
        bool adminExists = false;
        for (const auto &acc: accounts) {
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

    void adminFunctions(Register &account) {
        vector<Account *> &accounts = account.getAccountDetails();
        string choice;

        do {
            userInterface();

            cout << "\nChoice: ";
            getline(cin, choice);
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
                getline(cin, usernameToDelete);

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
    Staff() {
    }

    Staff(string username, string password) : Account(username, password, "Staff") {
    }

    void userInterface() const override {
        system("cls");
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
    RegularUser() {
    }

    RegularUser(string username, string password, string type, string name, string number, string address) : Account(
        username, password, "RegularUser") {
        this->name = name;
        this->number = number;
        this->address = address;
    }

    string getName() const { return name; }
    string getNumber() const { return number; }
    string getAddress() const { return address; }

    void setName(const string &newName) {
        name = newName;
    }

    void setNumber(const string &newNumber) {
        number = newNumber;
    }

    void setAddress(const string &newAddress) {
        address = newAddress;
    }


    void userInterface() const override {
        system("cls");
        cout << "a) Adopt a Pet" << endl;
        cout << "b) Add a Pet" << endl; //Class PetList - addPet() Function
        cout << "c) View All Pets" << endl; //Class PetList - viewAllPets() Function
        cout << "d) View Pets You Added" << endl;
        cout << "e) View Pets You Adopted" << endl;
        cout << "f) Edit Profile" << endl;
        cout << "x) Log Out" << endl << endl;
    }

    void inputPersonalInformation(Register &account) {
        vector<Account *> &accounts = account.getAccountDetails();

        cout << "Name: ";
        getline(cin, name);
        cout << "Mobile Number: ";
        getline(cin, number);
        cout << "Address: ";
        getline(cin, address);
        accounts.push_back(new RegularUser(getUsername(), getPassword(), getAccountType(), name, number, address));
    }

    void editPersonalInformation(Account *account, vector<Account *> &accounts) {
        RegularUser *currentAccount = dynamic_cast<RegularUser *>(account);

        string newName = currentAccount->getName();
        string newNumber = currentAccount->getNumber();
        string newAddress = currentAccount->getAddress();

        cout << "\nEdit Personal Information" << endl;

        cout << "Current Name: " << newName << "\nEnter new name (or press Enter to keep): ";
        getline(cin, newName);
        cout << "Current Mobile Number: " << newNumber << "\nEnter new mobile number (or press Enter to keep): ";
        getline(cin, newNumber);
        cout << "Current Address: " << newAddress << "\nEnter new address (or press Enter to keep): ";
        getline(cin, newAddress);

        // Apply only if user entered new data
        if (!newName.empty()) currentAccount->setName(newName);
        if (!newNumber.empty()) currentAccount->setNumber(newNumber);
        if (!newAddress.empty()) currentAccount->setAddress(newAddress);

        // Update the actual object in the accounts vector
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i]->getUsername() == currentAccount->getUsername()) {
                // Replace with updated object (optional: only if you recreate it)
                delete accounts[i];

                accounts[i] = new RegularUser(
                    currentAccount->getUsername(),
                    currentAccount->getPassword(),
                    currentAccount->getAccountType(),
                    currentAccount->getName(),
                    currentAccount->getNumber(),
                    currentAccount->getAddress()
                );

                cout << "Profile updated successfully!" << endl;
                break;
            }
        }
    }
};

vector<Account *> AccountsFileHandler::loadAccounts() {
    vector<Account *> accounts;
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

        Account *acc = nullptr;
        if (accountType == "Admin") {
            acc = new Admin(username, password);
        } else if (accountType == "Staff") {
            acc = new Staff(username, password);
        } else if (accountType == "RegularUser") {
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

void AccountsFileHandler::saveAccounts(vector<Account *> &accounts) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Failed to open " << filename << endl;
        return;
    }

    for (const Account *acc: accounts) {
        file << acc->getUsername() << "-"
                << acc->getPassword() << "-"
                << acc->getAccountType();

        // Handle RegularUser-specific fields
        if (const RegularUser *user = dynamic_cast<const RegularUser *>(acc)) {
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
        getline(cin, username);

        // Check if username already exists
        for (const auto &acc: accounts) {
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
        while ((ch = _getch()) != '\r') {
            // Stops when Enter is pressed
            if (ch == '\b') {
                // Backspace key
                if (!password.empty()) {
                    password.pop_back(); // Remove last character
                    cout << "\b \b"; // Erase '*' from console
                }
            } else {
                password += ch;
                cout << '*'; // Mask input with '*'
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

    Account *newAccount = nullptr;

    if (accountType == "Admin") {
        newAccount = new Admin(username, password);
    } else if (accountType == "Staff") {
        newAccount = new Staff(username, password);
    } else if (accountType == "RegularUser") {
        string name, number, address;
        // Clear newline from previous input
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
        for (const auto &account: accounts) {
            cout << setw(25) << left << account->getUsername()
                    << setw(30) << left << account->getPassword()
                    << setw(20) << left << account->getAccountType() << endl;
        }
    } else {
        for (const auto &account: accounts) {
            if (account->getAccountType() == type) {
                cout << setw(25) << left << account->getUsername()
                        << setw(30) << left << account->getPassword()
                        << setw(20) << left << account->getAccountType() << endl;
            }
        }
    }
    cout << endl;
}

vector<Account *> &Register::getAccountDetails() {
    return accounts;
}


class LogIn {
private:
    string username;
    string password;

public:
    Account *logIn(Register &account) {
        while (true) {
            cout << "Log In" << endl;
            cout << "Username: ";
            getline(cin, username);

            vector<Account *> &accounts = account.getAccountDetails();
            Account *foundAccount = nullptr;

            for (auto acc: accounts) {
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
    string requestedOrAdoptedBy;

public:
    Pet(string name, float age, string type, string status, string submittedBy, string requestedOrAdoptedBy) {
        this->name = name;
        this->age = age;
        this->type = type;
        this->status = status;
        this->submittedBy = submittedBy;
        this->requestedOrAdoptedBy = requestedOrAdoptedBy;
    }

    string getRequestedOrAdoptedBy() const {
        return requestedOrAdoptedBy;
    }

    void setRequestedOrAdoptedBy(string requestedOrAdoptedBy) {
        this->requestedOrAdoptedBy = requestedOrAdoptedBy;
    }

    string getStatus() const {
        return status;
    }

    string getSubmittedBy() const {
        return submittedBy;
    }

    void setStatus(string status) {
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
    Dog(string name, float age, string type, string breed, string status, string submittedBy,
        string requestedOrAdoptedBy) : Pet(name, age, "Dog", status, submittedBy, requestedOrAdoptedBy) {
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
    Cat(string name, float age, string type, string breed, string status, string submittedBy,
        string requestedOrAdoptedBy) : Pet(name, age, "Cat", status, submittedBy, requestedOrAdoptedBy) {
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
    static PetFileHandler *instance;

    // Private constructor for Singleton
    PetFileHandler(const string &filename) : filename(filename) {
    }

    // Prevent copying
    PetFileHandler(const PetFileHandler &) = delete;

    PetFileHandler &operator=(const PetFileHandler &) = delete;

public:
    // Singleton access
    static PetFileHandler *getInstance(const string &filename = "txt-file-storage/pets.txt") {
        if (!instance) {
            instance = new PetFileHandler(filename);
        }
        return instance;
    }

    // Save all pets to file
    void savePets(const vector<Pet *> &pets) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Failed to open " << filename << endl;
            return;
        }

        for (const Pet *pet: pets) {
            file << pet->getName() << "-"
                    << pet->getAge() << "-"
                    << pet->getType();


            // Handle Dog-specific fields
            if (const Dog *dog = dynamic_cast<const Dog *>(pet)) {
                file << "-" << dog->getBreed();
            }
            // Handle Cat-specific fields
            else if (const Cat *cat = dynamic_cast<const Cat *>(pet)) {
                file << "-" << cat->getBreed();
            }
            file << "-" << pet->getStatus() << "-"
                    << pet->getSubmittedBy() << "-"
                    << pet->getRequestedOrAdoptedBy();

            file << "\n";
        }
        file.close();
    }

    // Load all pets from file
    vector<Pet *> loadPets() {
        vector<Pet *> pets;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Note: " << filename << " not found. Starting fresh." << endl;
            return pets;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, ageStr, type, breed, status, submittedBy, requestedOrAdoptedBy;
            float age;

            getline(ss, name, '-');
            getline(ss, ageStr, '-');
            getline(ss, type, '-');
            getline(ss, breed, '-');
            getline(ss, status, '-');
            getline(ss, submittedBy, '-');
            getline(ss, requestedOrAdoptedBy, '-');


            try {
                age = stof(ageStr);
            } catch (const invalid_argument &e) {
                cerr << "Error: Invalid age format for pet " << name << endl;
                continue; // Skip this record
            }

            Pet *pet = nullptr;
            if (type == "Dog") {
                pet = new Dog(name, age, type, breed, status, submittedBy, requestedOrAdoptedBy);
            } else if (type == "Cat") {
                pet = new Cat(name, age, type, breed, status, submittedBy, requestedOrAdoptedBy);
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
PetFileHandler *PetFileHandler::instance = nullptr;


class PetList {
private:
    vector<Pet *> listOfPets; //List of Pets
    PetFileHandler *fileHandler;

    string getBreed(const Pet *pet) const {
        if (auto dog = dynamic_cast<const Dog *>(pet)) {
            return dog->getBreed();
        } else if (auto cat = dynamic_cast<const Cat *>(pet)) {
            return cat->getBreed();
        }
        return "Unknown";
    }

    static bool compareByAgeAscending(Pet *a, Pet *b) {
        return a->getAge() < b->getAge();
    }

    static bool compareByAgeDescending(Pet *a, Pet *b) {
        return a->getAge() > b->getAge();
    }

    vector<Pet *> getPendingPets() const {
        vector<Pet *> pending;
        for (Pet *pet: listOfPets) {
            if (pet->getStatus() == "pending") {
                pending.push_back(pet); // Stores pointer to original
            }
        }
        return pending;
    }

    vector<Pet *> getAvailablePets() const {
        vector<Pet *> availablePets;
        for (Pet *pet: listOfPets) {
            if (pet->getStatus() == "available") {
                availablePets.push_back(pet);
            }
        }
        return availablePets;
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
        for (auto pet: listOfPets) {
            delete pet;
        }
    }

    void viewAndApprovePets() {
        vector<Pet *> pendingPets = getPendingPets();

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
            Pet *pet = pendingPets[i];
            cout << setw(5) << left << i + 1
                    << setw(20) << left << pet->getName()
                    << setw(10) << left << pet->getAge()
                    << setw(15) << left << pet->getType()
                    << setw(20) << left << getBreed(pet)
                    << setw(20) << left << pet->getSubmittedBy() << endl;
        }
        cout << string(90, '-') << endl << endl;

        string choiceStr;
        cout << "Enter pet number to approve (or type 'x' to cancel): ";
        getline(cin, choiceStr);

        if (choiceStr == "x") {
            cout << "Approval cancelled.\n";
            system("pause");
            return;
        }

        // Validate input is a number
        if (!all_of(choiceStr.begin(), choiceStr.end(), ::isdigit)) {
            cout << "Invalid input. Please enter a valid number.\n";
            system("pause");
            return;
        }

        int choice = stoi(choiceStr); // Safe to convert now

        if (choice >= 1 && static_cast<size_t>(choice) <= pendingPets.size()) {
            pendingPets[choice - 1]->setStatus("available");
            fileHandler->savePets(listOfPets); // Save changes
            cout << "\nApproved: " << pendingPets[choice - 1]->getName() << endl;
        } else {
            cout << "Invalid selection.\n";
        }

        system("pause");
    }


    void addPet(const Account *account) {
        string accountType = account->getAccountType();
        cout << "Please Fill Out The Details:" << endl;
        cout << "Add A Pet:" << endl;

        string name, typeInput, breed, ageInput;
        float age = 0;

        cout << "Name: ";
        // Clear input buffer
        getline(cin, name);

        cout << "Age: ";
        getline(cin, ageInput);

        // Validate age (must be a number)
        try {
            age = stof(ageInput);
            if (age <= 0) throw invalid_argument("Age must be positive.");
        } catch (exception &e) {
            cout << "Invalid age entered. Please enter a valid positive number.\n";
            system("pause");
            return;
        }

        cout << "Type (A = Dog, B = Cat): ";
        getline(cin, typeInput);

        string status, submittedBy;
        if (accountType == "RegularUser") {
            status = "pending";
            submittedBy = account->getUsername();
        } else {
            status = "available";
            submittedBy = "Staff";
        }

        Pet *newPet = nullptr;

        if (typeInput == "A") {
            cout << "Dog's Breed: ";
            getline(cin, breed);
            newPet = new Dog(name, age, "Dog", breed, status, submittedBy, "none");
        } else if (typeInput == "B") {
            cout << "Cat's Breed: ";
            getline(cin, breed);
            newPet = new Cat(name, age, "Cat", breed, status, submittedBy, "none");
        } else {
            cout << "Invalid pet type. Please enter A or B.\n";
            system("pause");
            return;
        }

        listOfPets.push_back(newPet);
        fileHandler->savePets(listOfPets);
        system("cls");
        cout << "Pet added successfully:\n";
        newPet->viewPet();
        cout << endl << endl;
        system("pause");
        system("cls");
    }


    void deletePet() {
        cout << string(130, '-') << endl;
        cout << "\t\t\t\t\t\t\t\tDelete Pet" << endl;
        cout << string(130, '-') << endl;

        cout << setw(5) << left << "No."
                << setw(20) << left << "Name"
                << setw(10) << left << "Age"
                << setw(15) << left << "Type"
                << setw(25) << left << "Breed"
                << setw(20) << left << "Status"
                << setw(20) << left << "Submitted By"
                << setw(15) << left << "Requested By" << endl;
        cout << string(130, '-') << endl;

        for (int i = 0; i < listOfPets.size(); ++i) {
            Pet *pet = listOfPets[i];
            cout << setw(5) << left << i + 1
                    << setw(20) << left << pet->getName()
                    << setw(10) << left << pet->getAge()
                    << setw(15) << left << pet->getType()
                    << setw(25) << left << getBreed(pet)
                    << setw(20) << left << pet->getStatus()
                    << setw(20) << left << pet->getSubmittedBy()
                    << setw(15) << left << pet->getRequestedOrAdoptedBy() << endl;
        }

        cout << string(130, '-') << endl;

        // Handle numeric input with getline
        string inputStr;
        int choice = 0;
        cout << "\nEnter the number of the pet to delete (0 to cancel): ";
        getline(cin, inputStr);
        try {
            choice = stoi(inputStr);
        } catch (...) {
            choice = -1;
        }

        if (choice <= 0 || choice > listOfPets.size()) {
            cout << "Action cancelled or invalid selection." << endl;
            system("pause");
            system("cls");
            return;
        }

        cout << "\nAre you sure you want to delete " << listOfPets[choice - 1]->getName() << "? (Y/N): ";
        string confirm;
        getline(cin, confirm);

        if (confirm != "Y" && confirm != "y") {
            cout << "Action cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        if (choice >= 1 && static_cast<size_t>(choice) <= listOfPets.size()) {
            string name = listOfPets[choice-1]->getName();
            listOfPets.erase(listOfPets.begin() + (choice - 1));
            fileHandler->savePets(listOfPets); // Save changes
            cout << "Deleted Pet: " << name << endl;
        } else {
            cout << "Invalid selection.\n";
        }

        system("pause");
    }

    void viewAllPets(string type) const {
        vector<Pet *> availablePets = getAvailablePets();

        system("cls");
        if (availablePets.empty()) {
            cout << "No pets available." << endl << endl;
            system("pause");
            system("cls");
            return;
        }

        cout << string(80, '-') << endl;
        if (type == "All") {
            cout << "\t\t\tAvailable Pets" << endl;
        } else {
            cout << "\t\t\tAvailable " << type << "s" << endl;
        }
        cout << string(80, '-') << endl;

        cout << setw(5) << left << "No."
                << setw(20) << left << "Name"
                << setw(10) << left << "Age"
                << setw(15) << left << "Type"
                << setw(25) << left << "Breed" << endl;
        cout << string(80, '-') << endl;

        int count = 0;
        vector<Pet *> filteredPets;

        for (auto pet: availablePets) {
            if (type == "All" || pet->getType() == type) {
                cout << setw(5) << left << count + 1
                        << setw(20) << left << pet->getName()
                        << setw(10) << left << pet->getAge()
                        << setw(15) << left << pet->getType()
                        << setw(25) << left << getBreed(pet) << endl;
                filteredPets.push_back(pet);
                ++count;
            }
        }

        if (count == 0 && (type == "All" || type == "Dog" || type == "Cat")) {
            cout << "No pets of this type available." << endl;
            system("pause");
            system("cls");
        }
    }

    void viewPetsSortedByAge(bool ascending = true) const {
        vector<Pet *> sortedPets = getAvailablePets();

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
        for (const auto &pet: sortedPets) {
            cout << setw(25) << left << pet->getName()
                    << setw(25) << left << pet->getAge()
                    << setw(25) << left << pet->getType()
                    << setw(25) << left << getBreed(pet) << endl;
        }

        cout << string(80, '-') << endl << endl;
    }

    void viewAndAdoptPet(string type, Account *currentAccount) const{
        vector<Pet *> availablePets = getAvailablePets();
        vector<Pet *> filteredPets;

        for (auto pet: availablePets) {
            if (type == "All" || pet->getType() == type) {
                filteredPets.push_back(pet);
            }
        }

        system("cls");

        if (filteredPets.empty()) {
            cout << "No pets available." << endl;
            system("pause");
            system("cls");
            return;
        }

        // Display the filtered pets
        cout << string(80, '-') << endl;
        cout << "\t\t\tAdoptable " << (type == "All" ? "Pets" : type + "s") << endl;
        cout << string(80, '-') << endl;
        cout << setw(5) << left << "No."
                << setw(20) << left << "Name"
                << setw(10) << left << "Age"
                << setw(15) << left << "Type"
                << setw(25) << left << "Breed" << endl;
        cout << string(80, '-') << endl;

        for (size_t i = 0; i < filteredPets.size(); ++i) {
            Pet *pet = filteredPets[i];
            cout << setw(5) << left << i + 1
                    << setw(20) << left << pet->getName()
                    << setw(10) << left << pet->getAge()
                    << setw(15) << left << pet->getType()
                    << setw(25) << left << getBreed(pet) << endl;
        }

        cout << string(80, '-') << endl;

        if (currentAccount->getAccountType() != "RegularUser") {
            system("pause");
            system("cls");
            return;
        }

        // Adoption prompt
        string selectionInput;
        cout << "\nEnter the number of the pet to adopt (0 to cancel): ";
        getline(cin, selectionInput);

        int selection = 0;
        try {
            selection = stoi(selectionInput);
        } catch (...) {
            cout << "Invalid input. Adoption cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        if (selection <= 0 || selection > (int) filteredPets.size()) {
            cout << "Adoption cancelled or invalid selection." << endl;
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = filteredPets[selection - 1];

        cout << "\nYou selected: " << selectedPet->getName()
                << " (" << selectedPet->getType() << ", "
                << getBreed(selectedPet) << ")" << endl;

        string confirmInput;
        cout << "Confirm adoption? (y/n): ";
        getline(cin, confirmInput);

        if (!confirmInput.empty() && tolower(confirmInput[0]) == 'y') {
            selectedPet->setStatus("pendingAdoption");
            selectedPet->setRequestedOrAdoptedBy(currentAccount->getUsername());
            cout << "Adoption request submitted. Pending staff approval." << endl;

            fileHandler->savePets(listOfPets);
        } else {
            cout << endl << "Adoption cancelled." << endl << endl;
        }

        system("pause");
        system("cls");
    }

    void filterPet(Account *currentAccount) const {
        bool continueFiltering = true;

        while (continueFiltering) {
            system("cls");
            vector<Pet *> allPets = getAvailablePets();

            viewAllPets("All");

            cout << endl << "Filter Pets (enter pet number to adopt):" << endl;
            cout << "a) By Type" << endl;
            cout << "b) By Age" << endl;
            cout << "x) Return to menu" << endl;
            cout << "\nChoice: ";
            string input;
            getline(cin, input);

            if (input == "x") {
                continueFiltering = false;
            } else if (input == "a") {
                // Filter by type
                system("cls");
                cout << "Choose a Type (Input x to return to menu):" << endl;
                cout << "a) Dogs" << endl;
                cout << "b) Cats" << endl;
                cout << "Choice: ";
                string type;
                getline(cin, type);
                if (type == "a") viewAndAdoptPet("Dog", currentAccount);
                else if (type == "b") viewAndAdoptPet("Cat", currentAccount);
                else if (type == "x") continue;
                else {
                    cout << "Invalid type. Press Enter to continue." << endl;
                    cin.ignore();
                }
            } else if (input == "b") {
                // Filter by age
                system("cls");
                cout << "Sort by Age (Input x to return to menu):" << endl;
                cout << "a) Ascending" << endl;
                cout << "b) Descending" << endl;
                cout << "Choice: ";
                string ageOrder;
                getline(cin, ageOrder);
                if (ageOrder == "a") viewPetsSortedByAge(true);
                else if (ageOrder == "b") viewPetsSortedByAge(false);
                else if (ageOrder == "x") continue;
                else {
                    cout << "Invalid input. Press Enter to continue." << endl;
                    cin.ignore();
                }
                system("pause");
            } else {
                // Try to parse it as a number for adoption
                try {
                    int petNum = stoi(input);
                    vector<Pet *> allPets = getAvailablePets();

                    if (petNum >= 1 && petNum <= (int) allPets.size()) {
                        Pet *selectedPet = allPets[petNum - 1];
                        if (selectedPet->getStatus() == "available") {
                            cout << "\nYou selected: " << selectedPet->getName()
                                    << " (" << selectedPet->getType() << ", "
                                    << getBreed(selectedPet) << ")" << endl;
                            cout << "Confirm adoption? (y/n): ";
                            string confirmInput;
                            getline(cin, confirmInput);

                            if (!confirmInput.empty() && tolower(confirmInput[0]) == 'y') {
                                selectedPet->setStatus("pendingAdoption");
                                selectedPet->setRequestedOrAdoptedBy(currentAccount->getUsername());
                                cout << "Adoption request submitted. Pending staff approval." << endl;
                                fileHandler->savePets(listOfPets);
                            } else {
                                cout << "Adoption cancelled." << endl;
                            }
                            system("pause");
                        } else {
                            cout << "This pet is not available for adoption." << endl;
                            system("pause");
                        }
                    } else {
                        cout << "Invalid pet number." << endl;
                        system("pause");
                    }
                } catch (...) {
                    cout << "Invalid input. Please try again." << endl;
                    system("pause");
                }
            }
        }
    }


    void viewAndApproveAdoptions() {
        vector<Pet *> pendingPets;
        for (Pet *pet: listOfPets) {
            if (pet->getStatus() == "pendingAdoption") {
                pendingPets.push_back(pet);
            }
        }

        system("cls");
        if (pendingPets.empty()) {
            cout << "No pending adoption requests at the moment.\n" << endl;
            system("pause");
            system("cls");
            return;
        }

        cout << string(80, '-') << endl;
        cout << "\t\tPending Adoption Requests" << endl;
        cout << string(80, '-') << endl;

        cout << setw(5) << left << "No."
                << setw(20) << left << "Name"
                << setw(10) << left << "Age"
                << setw(15) << left << "Type"
                << setw(25) << left << "Breed"
                << setw(15) << left << "Requested By" << endl;
        cout << string(80, '-') << endl;

        for (int i = 0; i < pendingPets.size(); ++i) {
            Pet *pet = pendingPets[i];
            cout << setw(5) << left << i + 1
                    << setw(20) << left << pet->getName()
                    << setw(10) << left << pet->getAge()
                    << setw(15) << left << pet->getType()
                    << setw(25) << left << getBreed(pet)
                    << setw(15) << left << pet->getRequestedOrAdoptedBy() << endl;
        }

        cout << string(80, '-') << endl;

        // Handle numeric input with getline
        string inputStr;
        int choice = 0;
        cout << "\nEnter the number of the pet to manage (0 to cancel): ";
        getline(cin, inputStr);
        try {
            choice = stoi(inputStr);
        } catch (...) {
            choice = -1;
        }

        if (choice <= 0 || choice > pendingPets.size()) {
            cout << "Action cancelled or invalid selection." << endl;
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = pendingPets[choice - 1];

        // Handle approval/rejection input
        cout << "\nApprove (A) or Reject (R) this adoption request? (A/R): ";
        string decisionStr;
        getline(cin, decisionStr);

        if (decisionStr.length() == 1) {
            char decision = tolower(decisionStr[0]);

            if (decision == 'a') {
                selectedPet->setStatus("adopted");
                cout << "Adoption approved successfully!" << endl;
            } else if (decision == 'r') {
                selectedPet->setStatus("available");
                selectedPet->setRequestedOrAdoptedBy("none");
                cout << "Adoption request rejected. Pet is now available again." << endl;
            } else {
                cout << "Invalid input. No action taken." << endl;
            }
        } else {
            cout << "Invalid input. No action taken." << endl;
        }

        fileHandler->savePets(listOfPets);
        system("pause");
        system("cls");
    }

    void viewPetsAddedByUser(const Account *currentAccount) const {
        string username = currentAccount->getUsername();
        vector<Pet *> userPets;

        for (Pet *pet: listOfPets) {
            if (pet->getSubmittedBy() == username) {
                userPets.push_back(pet);
            }
        }

        system("cls");
        cout << "\nPets You Have Added:\n";
        cout << string(90, '-') << endl;

        if (userPets.empty()) {
            cout << "You have not added any pets yet.\n";
        } else {
            cout << setw(20) << left << "Name"
                    << setw(10) << left << "Age"
                    << setw(15) << left << "Type"
                    << setw(20) << left << "Breed"
                    << setw(20) << left << "Status" << endl;
            cout << string(90, '-') << endl;

            int count = 1;
            for (Pet *pet: userPets) {
                cout << setw(20) << left << pet->getName()
                        << setw(10) << left << pet->getAge()
                        << setw(15) << left << pet->getType()
                        << setw(20) << left << getBreed(pet)
                        << setw(20) << left << pet->getStatus() << endl;
            }
        }

        cout << endl;
        system("pause");
    }

    void viewPetsAdoptedByUser(const Account *currentAccount) const {
        string username = currentAccount->getUsername();
        vector<Pet *> adoptedPets;

        for (Pet *pet: listOfPets) {
            if (pet->getRequestedOrAdoptedBy() == username &&
                (pet->getStatus() == "adopted" || pet->getStatus() == "pendingAdoption")) {
                adoptedPets.push_back(pet);
            }
        }

        system("cls");
        cout << "\nPets You Have Adopted or Requested:\n";
        cout << string(90, '-') << endl;

        if (adoptedPets.empty()) {
            cout << "You have not adopted or requested to adopt any pets yet.\n";
        } else {
            cout << setw(20) << left << "Name"
                    << setw(10) << left << "Age"
                    << setw(15) << left << "Type"
                    << setw(20) << left << "Breed"
                    << setw(20) << left << "Status" << endl;
            cout << string(90, '-') << endl;

            int count = 1;
            for (Pet *pet: adoptedPets) {
                cout << setw(20) << left << pet->getName()
                        << setw(10) << left << pet->getAge()
                        << setw(15) << left << pet->getType()
                        << setw(20) << left << getBreed(pet)
                        << setw(20) << left << pet->getStatus() << endl;
            }
        }

        cout << endl;
        system("pause");
    }
};

int main() {
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
    vector<Account *> accounts = filehandler->loadAccounts();

    PetList animal;

    string choice;

    do {
        cout << "[DEBUG] Still inside filter menu loop\n";

        Account *currentAccount = nullptr;
        cout << "Pet Adoption System" << endl;
        cout << "a) Register" << endl;
        cout << "b) Login" << endl;
        cout << "c) Exit" << endl << endl;
        cout << "Enter choice: ";
        getline(cin, choice);

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
                    getline(cin, choice);
                    system("cls");
                    if (choice == "a") {
                        animal.addPet(currentAccount);
                    } else if (choice == "b") {
                        animal.deletePet(); // LETTER C = add view pet adoption request - amaya
                    } else if (choice == "c") {
                        animal.viewAndApproveAdoptions();
                    } else if (choice == "d") {
                        animal.viewAllPets("All");
                        cout << endl;
                        system("pause");
                    } else if (choice == "e") {
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
                    getline(cin, choice);
                    system("cls");

                    if (choice == "a") {// Adopt a Pet (View + Adopt)
                        animal.filterPet(currentAccount);
                    } else if (choice == "b") {// Add Pet (requires staff approval)
                        animal.addPet(currentAccount);
                    } else if (choice == "c") {// view all pets (no adoption)
                        animal.viewAllPets("All");
                        cout << endl;
                        system("pause");
                    } else if (choice == "d") {// View pets added by user
                        animal.viewPetsAddedByUser(currentAccount);
                    } else if (choice == "e") {// View adopted pets
                        animal.viewPetsAdoptedByUser(currentAccount);
                    } else if (choice == "f") {// Edit account info
                        userAccount.editPersonalInformation(currentAccount, accounts);
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
