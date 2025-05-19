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

string getInput(string prompt) {
    string input;
    cout << prompt; //Print the prompt
    getline(cin, input); //Get user input
    return input;
}

int getInputInt(string prompt, int minVal, int maxVal) {
    string input;
    int output;
    bool isValid = false;
    do {
        input = getInput(prompt);
        try {
            output = stoi(input);
            if (output < minVal || output > maxVal) {
                //Check if the input is out of range
                throw out_of_range(
                    "Please enter a number between " + to_string(minVal) + " and " + to_string(maxVal) + ".");
            }
            isValid = true;
        } catch (invalid_argument &e) {
            cout << "Error: Please enter a valid number." << endl << endl;
        }catch (out_of_range &e) {
            cout << "Error: " << e.what() << endl << endl;
        }
    } while (!isValid);
    return output;
}

int getInputInt(string prompt) {
    string input;
    int output;
    bool isValid = false;
    do {
        input = getInput(prompt);
        try {
            output = stoi(input);
            if (output < 0) {
                throw out_of_range("Please input a value greater than 0.");
            }
            isValid = true;
        } catch (invalid_argument &e) {
            cout << "Error: Please enter a valid number." << endl << endl;
        }catch (out_of_range &e) {
            cout << "Error: " << e.what() << endl << endl;
        }
    } while (!isValid);
    return output;
}

string getValidatedString(string text) {
    string input;
    bool valid = false; // Initialize valid to false

    do {
        valid = true;
        cout << text;
        getline(cin, input); // Read the entire line as a string

        try {
            if (input.empty()) {
                throw invalid_argument("Empty string not allowed");
            }

            // Check if string is only whitespace
            bool allWhitespace = true;
            for (char c: input) {
                if (!isspace(c)) {
                    allWhitespace = false;
                    break;
                }
            }
            if (allWhitespace) {
                throw invalid_argument("Input is empty");
            }

            // Convert alphabetic characters to lowercase and validate characters
            for (char &c: input) {
                if (!isalnum(c) && c != ' ') {
                    throw invalid_argument("Only alphanumeric characters and spaces are allowed.");
                }
                c = tolower(c);
            }
            // Input is valid after conversion
        } catch (const exception &e) {
            valid = false;
            cout << "Error: " << e.what() << endl; // Print the error message
        }
    } while (!valid);

    return input;
}

class Account;
class Admin;
class Staff;
class RegularUser;
class AccountsFileHandler;
class PetFileHandler;
class PetList;
class Pet;
class Cat;
class Dog;

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

class PetFileHandler {
private:
    string filename;
    static PetFileHandler *instance;

    // Private constructor for Singleton
    PetFileHandler(const string &filename);

    // Prevent copying
    PetFileHandler(const PetFileHandler &) = delete;

    PetFileHandler &operator=(const PetFileHandler &) = delete;

public:
    // Singleton access
    static PetFileHandler *getInstance(const string &filename = "txt-file-storage/pets.txt");

    // Update username in all pet records
    static void updateUsernameInPets(const string &oldUsername, const string &newUsername);

    // Save all pets to file
    void savePets(const vector<Pet *> &pets);

    // Load all pets from file
    vector<Pet *> loadPets();

    // Cleanup Singleton instance
    static void cleanup();
};

AccountsFileHandler *AccountsFileHandler::instance = nullptr;
PetFileHandler *PetFileHandler::instance = nullptr;


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
    vector<Account *> accounts;
    AccountsFileHandler *fileHandler;
    string usernameToDelete;

public:
    Admin(): fileHandler(AccountsFileHandler::getInstance()) {
    }

    Admin(string username, string password) : Account(username, password, "Admin"), fileHandler(AccountsFileHandler::getInstance()) {
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

            choice = getValidatedString("\nChoice: ");
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
                // Delete Account
                account.displayRegisteredAccounts("All Accounts", "All");
                usernameToDelete = getValidatedString("\nEnter the username of the account to delete: ");

                bool accountFound = false;
                bool isAdminAccount = false;
                size_t accountIndex = 0;

                // Find the account
                for (size_t i = 0; i < accounts.size(); ++i) {
                    if (accounts[i]->getUsername() == usernameToDelete) {
                        accountFound = true;
                        accountIndex = i;
                        if (accounts[i]->getAccountType() == "Admin") {
                            isAdminAccount = true;
                        }
                        break;
                    }
                }

                if (!accountFound) {
                    cout << "Error: Account '" << usernameToDelete << "' not found." << endl;
                } else if (isAdminAccount) {
                    cout << "Error: Cannot delete admin accounts. Operation denied." << endl;
                } else {
                    // Confirm deletion
                    string confirm;
                    cout << "Are you sure you want to delete account '" << usernameToDelete << "'? (y/n): ";
                    getline(cin, confirm);
                    
                    if (confirm == "y" || confirm == "Y") {
                        // Update pets that might reference this user
                        PetFileHandler::updateUsernameInPets(usernameToDelete, "deleted_user");
                        
                        // Perform the deletion
                        delete accounts[accountIndex];
                        accounts.erase(accounts.begin() + accountIndex);
                        
                        // Save changes
                        fileHandler->saveAccounts(accounts);
                        cout << "Account: '" << usernameToDelete << "' has been deleted." << endl;
                    } else {
                        cout << "Deletion cancelled." << endl;
                    }
                }
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
        cout << "d) View Pending Pet Submissions" << endl;
        cout << "e) View All Pets for Adoption" << endl;
        cout << "g) Edit Pets" << endl;
        cout << "x) Log Out" << endl << endl;
    }
};

class RegularUser : public Account {
private:
    string name;
    string number;
    string address;

    bool editInformation(string oldString, string newString) {
        if (newString.empty()) {
            return false;
        }

        bool allWhitespace = true;
        for (char c: newString) {
            if (!isspace(c)) {
                allWhitespace = false;
                break;
            }
        }

        if (allWhitespace) {
            return false;
        }

        if (oldString == newString) {
            return false;
        }

        return true;
    }

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

        name = getValidatedString("Name: ");
        number = getValidatedString("Mobile Number: ");
        address = getValidatedString("Address: ");

        accounts.push_back(new RegularUser(getUsername(), getPassword(), getAccountType(), name, number, address));
    }

    void editPersonalInformation(Account *account, vector<Account *> &accounts, PetFileHandler *petHandler = nullptr) {
        RegularUser *currentAccount = dynamic_cast<RegularUser *>(account);

        string newName;
        string newNumber;
        string newAddress;

        cout << "\nEdit Personal Information" << endl;

        cout << "Current Name: " << currentAccount->getName() << "\nEnter new name (or press Enter to keep): ";
        getline(cin, newName);
        cout << "Current Mobile Number: " << currentAccount->getNumber() <<
                "\nEnter new mobile number (or press Enter to keep): ";
        getline(cin, newNumber);
        cout << "Current Address: " << currentAccount->getAddress() << "\nEnter new address (or press Enter to keep): ";
        getline(cin, newAddress);

        if (editInformation(currentAccount->getName(), newName)) {
            currentAccount->setName(newName);
            if (!petHandler) petHandler = PetFileHandler::getInstance();
            vector<Pet *> pets = petHandler->loadPets();
            petHandler->savePets(pets);
        }

        if (editInformation(currentAccount->getAddress(), newAddress)) {
            currentAccount->setAddress(newAddress);
        }

        if (editInformation(currentAccount->getNumber(), newNumber)) {
            currentAccount->setNumber(newNumber);
        }

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
    }
    file.close();
}

// ———————— IMPLEMENTATION OF REGISTER METHODS —————————


void Register::addAccount(string accountType) {
    bool correctPassword;
    bool validUsername;
    bool isValidPassword;
    char ch;

    cout << "\t   Register" << endl << "(enter '0' at any time to cancel)" << endl << endl;

    do {
        validUsername = true;
        username = getValidatedString("Username: ");

        if (username == "0") {
            cout << endl << "Registration cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        if (username.find(' ') != string::npos) {
            cout << "Username must not contain spaces. Please try another." << endl;
            validUsername = false;
            continue;
        }

        if (username.length() < 5) {
            cout << "Username must be at least 5 characters long. Please try another." << endl;
            validUsername = false;
            continue;
        }

        for (const auto &acc: accounts) {
            if (acc->getUsername() == username) {
                cout << "Username Already Exists. Please try another." << endl;
                validUsername = false;
                break;
            }
        }
    } while (!validUsername);

    do {
        correctPassword = true;
        isValidPassword;
        password = "";
        confirmPassword = "";

        do {
            isValidPassword = true;
            password.clear();
            cout << "Password: ";
            while ((ch = _getch()) != '\r') {
                if (password.empty() && ch == '0') {
                    cout << endl << endl << "Registration cancelled." << endl;
                    system("pause");
                    system("cls");
                    return;
                }

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
            cout << endl;

            if (password.find(' ') != string::npos) {
                cout << "Password must not contain spaces. Please try another." << endl;
                validUsername = false;
                continue;
        }

            if (password == "0") {
                cout << endl << "Registration cancelled." << endl;
                system("pause");
                system("cls");
                return;
            }

            if (password.length() < 8) {
                cout << "Password must be at least 8 characters long." << endl;
                isValidPassword = false;
                continue;
            }

            int charCount = 0;
            int intCount = 0;
            for (char c: password) {
                if (isalpha(c)) {
                    charCount++;
                } else if (isdigit(c)) {
                    intCount++;
                }
            }

            if (charCount < 1 || intCount < 1) {
                cout << "Password must contain at least one letter and one digit." << endl;
                isValidPassword = false;
            }
        } while (!isValidPassword);

        cout << "Re-enter Password: ";
        while ((ch = _getch()) != '\r') {
            if (confirmPassword.empty() && ch == '0') {
                cout << endl << endl << "Registration cancelled." << endl;
                system("pause");
                system("cls");
                return;
            }

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

        if (confirmPassword == "0") {
            cout << endl << "Registration cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        if (password != confirmPassword) {
            cout << "\nPasswords do not match. Try again.\n";
            correctPassword = false;
        }
    } while (!correctPassword);

    Account *newAccount = nullptr;

    if (accountType == "Admin") {
        newAccount = new Admin(username, password);
    } else if (accountType == "Staff") {
        newAccount = new Staff(username, password);
    } else if (accountType == "RegularUser") {
        string name, number, address;
        cout << endl << endl << "Enter Personal Details" << endl;

            name = getValidatedString("Name: ");
            if (name == "0") {
                cout << endl << "Registration cancelled." << endl;
                system("pause");
                system("cls");
                return;
            }

            number = getValidatedString("Mobile Number: ");
            if (number == "0") {
                cout << endl << "Registration cancelled." << endl;
                system("pause");
                system("cls");
                return;
            }

            address = getValidatedString("Address: ");
            if (address == "0") {
                cout << endl << "Registration cancelled." << endl;
                system("pause");
                system("cls");
                return;
            }
        newAccount = new RegularUser(username, password, "RegularUser", name, number, address);
    } else {
        cout << "Invalid account type. Registration failed.\n";
        return;
    }

    accounts.push_back(newAccount);
    fileHandler->saveAccounts(accounts);
    cout << "\nAccount created successfully!" << endl;
    system ("pause");
    system ("cls");
}

void Register::displayRegisteredAccounts(string text, string type) const {
    cout << string(45, '-') << endl;
    cout << "\t   " << text << endl;
    cout << string(45, '-') << endl;
    cout << setw(25) << left << "Username"
            << setw(20) << left << "Account Type" << endl;
    cout << string(45, '-') << endl;

    if (type == "All") {
        for (const auto &account: accounts) {
            if (account->getAccountType() != "Admin") {
                cout << setw(25) << left << account->getUsername()
                        << setw(15) << left << account->getAccountType() << endl;
            }
        }
    } else {
        for (const auto &account: accounts) {
            if (account->getAccountType() == type) {
                cout << setw(25) << left << account->getUsername()
                        << setw(20) << left << account->getAccountType() << endl;
            }
        }
    }
    cout << string(45, '-') << endl;
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
        cout << "\t   Log In " << endl << "(enter '0' at any time to cancel)" << endl << endl;

        while (true) {
            username = getValidatedString("Username: ");

            // Check for cancellation
            if (username == "0") {
                cout << endl << endl << "Login cancelled." << endl;
                system("pause");
                system("cls");
                return nullptr;
            }

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
                continue; // Retry username
            }

            // Password entry loop
            while (true) {
                // Masked password input
                char ch;
                password = "";
                cout << "Password: ";
                while ((ch = _getch()) != '\r') {
                    if (password.empty() && ch == '0') {
                        cout << endl << endl << "Login cancelled." << endl;
                        system("pause");
                        system("cls");
                        return nullptr;
                    }

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

                if (password == "0") {
                    cout << endl << endl << "Login cancelled." << endl;
                    system("pause");
                    system("cls");
                    return nullptr;
                }

                if (password == foundAccount->getPassword()) {
                    cout << "\n\nLogin successful!" << endl;
                    system("pause");
                    return foundAccount;
                } else {
                    cout << "\nIncorrect password! Please try again." << endl;
                    // Stay in password loop to retry password
                }
            }
        }
        return nullptr; // Fallback (shouldn't be reached)
    }
};


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

    void setName(string name) {
        this->name = name;
    }

    void setAge(float age) {
        this->age = age;
    }

    void setType(string type) {
        this->type = type;
    }

    void setSubmittedBy(string submittedBy) {
        this->submittedBy = submittedBy;
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

class SortStrategy {
public:
    virtual void sort(vector<Pet*>& pets) = 0;
    virtual ~SortStrategy() = default;
};

// Age Ascending Sort Strategy
class AgeAscendingSort : public SortStrategy {
public:
    void sort(vector<Pet*>& pets) override {
        std::sort(pets.begin(), pets.end(), [](Pet* a, Pet* b) {
            return a->getAge() < b->getAge();
        });
    }
};

// Age Descending Sort Strategy
class AgeDescendingSort : public SortStrategy {
public:
    void sort(vector<Pet*>& pets) override {
        std::sort(pets.begin(), pets.end(), [](Pet* a, Pet* b) {
            return a->getAge() > b->getAge();
        });
    }
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

    void setBreed(string breed) {
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
    Cat(string name, float age, string type, string breed, string status, string submittedBy,
        string requestedOrAdoptedBy) : Pet(name, age, "Cat", status, submittedBy, requestedOrAdoptedBy) {
        this->breed = breed;
    }

    string getBreed() const { return breed; }

    void setBreed(string breed) {
        this->breed = breed;
    }

    void viewPet() const override {
        cout << "Cat Name: " << getName() << endl
                << "Age: " << getAge() << endl
                << "Breed: " << breed;
    }
};

// Private constructor implementation
PetFileHandler::PetFileHandler(const string &filename) : filename(filename) {
}

// Singleton access implementation
PetFileHandler *PetFileHandler::getInstance(const string &filename) {
    if (!instance) {
        instance = new PetFileHandler(filename);
    }
    return instance;
}

void PetFileHandler::updateUsernameInPets(const string &oldUsername, const string &newUsername) {
    PetFileHandler *instance = getInstance();
    vector<Pet *> pets = instance->loadPets();
    bool changed = false;

    for (Pet *pet: pets) {
        if (pet->getSubmittedBy() == oldUsername) {
            pet->setSubmittedBy(newUsername);
            changed = true;
        }
        if (pet->getRequestedOrAdoptedBy() == oldUsername) {
            pet->setRequestedOrAdoptedBy(newUsername);
            changed = true;
        }
    }

    if (changed) instance->savePets(pets);

    for (Pet *pet: pets) delete pet;
}

void PetFileHandler::savePets(const vector<Pet *> &pets) {
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

vector<Pet *> PetFileHandler::loadPets() {
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

void PetFileHandler::cleanup() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

class PetList {
private:
    SortStrategy* currentSortStrategy;
    static AgeAscendingSort ageAscending;
    static AgeDescendingSort ageDescending;
    vector<Pet *> listOfPets; //List of Pets
    PetFileHandler *fileHandler;

    void setBreed(Pet *pet, const string &breed) {
        if (pet == nullptr) return;

        if (auto dog = dynamic_cast<Dog *>(pet)) {
            dog->setBreed(breed);
        } else if (auto cat = dynamic_cast<Cat *>(pet)) {
            cat->setBreed(breed);
        }
    }

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

    bool editInformation(string oldString, string newString) {
        if (newString.empty()) {
            return false;
        }

        bool allWhitespace = true;
        for (char c: newString) {
            if (!isspace(c)) {
                allWhitespace = false;
                break;
            }
        }

        if (allWhitespace) {
            return false;
        }

        if (oldString == newString) {
            return false;
        }

        return true;
    }

    void displayPets(const vector<Pet*>& pets, const string& title, bool showNumbers = true) const {
        system("cls");
        if (pets.empty()) {
            cout << "No pets available." << endl << endl;
            system("pause");
            system("cls");
            return;
        }

        cout << string(80, '-') << endl;
        cout << "\t\t\t" << title << endl;
        cout << string(80, '-') << endl;

        if (showNumbers) {
            cout << setw(5) << left << "No.";
        }
        cout << setw(20) << left << "Name"
             << setw(10) << left << "Age"
             << setw(15) << left << "Type"
             << setw(25) << left << "Breed" << endl;
        cout << string(80, '-') << endl;

        for (size_t i = 0; i < pets.size(); ++i) {
            if (showNumbers) {
                cout << setw(5) << left << i + 1;
            }
            cout << setw(20) << left << pets[i]->getName()
                 << setw(10) << left << pets[i]->getAge()
                 << setw(15) << left << pets[i]->getType()
                 << setw(25) << left << getBreed(pets[i]) << endl;
        }

        cout << string(80, '-') << endl;
    }

    void handleTypeFilter(Account* currentAccount) {
        system("cls");
        cout << "Choose a Type (Input x to return to menu):\n"
             << "a) Dogs\n"
             << "b) Cats\n";

        string type = getValidatedString("Choice: ");

        if (type == "a") {
            viewAndAdoptPet("Dog", currentAccount);
        }
        else if (type == "b") {
            viewAndAdoptPet("Cat", currentAccount);
        }
        else if (type != "x") {
            cout << "Invalid type. Press Enter to continue.";
            cin.ignore();
            cin.get();
        }
    }

    void handleAgeFilter() {
        system("cls");
        cout << "Sort by Age (Input x to return to menu):\n"
             << "a) Ascending\n"
             << "b) Descending\n";

        string ageOrder = getValidatedString("Choice: ");

        if (ageOrder == "a") {
            currentSortStrategy = &ageAscending;
            displaySortedPets();
        }
        else if (ageOrder == "b") {
            currentSortStrategy = &ageDescending;
            displaySortedPets();
        }
        else if (ageOrder != "x") {
            cout << "Invalid input. Press Enter to continue.";
            cin.ignore();
            cin.get();
        }
    }

    void displaySortedPets() {
        vector<Pet*> petsToShow = getAvailablePets();
        currentSortStrategy->sort(petsToShow);

        string title = (currentSortStrategy == &ageAscending)
            ? "Age (Ascending)" : "Age (Descending)";

        displayPets(petsToShow, title);
        system("pause");
    }

    void handleAdoptionInput(const string& input, Account* currentAccount) {
        try {
            int petNum = stoi(input);
            vector<Pet*> allPets = getAvailablePets();

            if (petNum < 1 || petNum > static_cast<int>(allPets.size())) {
                throw out_of_range("Invalid pet number.");
            }

            Pet* selectedPet = allPets[petNum - 1];
            if (!confirmAdoption(selectedPet)) {
                cout << "Adoption cancelled.\n";
                system("pause");
                system("cls");
                return;
            }

            processAdoption(selectedPet, currentAccount);
            cout << "Adoption request submitted. Pending staff approval.\n";
            system("pause");
        }
        catch (const invalid_argument&) {
            cout << "Please enter a valid number.\n";
        }
        catch (const out_of_range& e) {
            cout << e.what() << "\n";
        }
    }

    bool confirmAdoption(Pet* pet) {
        cout << "\nYou selected: " << pet->getName()
             << " (" << pet->getType() << ", "
             << getBreed(pet) << ")\n"
             << "Confirm adoption? (y/n): ";

        string confirmInput;
        getline(cin, confirmInput);
        return (!confirmInput.empty() && tolower(confirmInput[0]) == 'y');
    }

    void processAdoption(Pet* pet, Account* currentAccount) {
        pet->setStatus("pendingAdoption");
        pet->setRequestedOrAdoptedBy(currentAccount->getUsername());
        fileHandler->savePets(listOfPets);
    }


public:

    PetList() : fileHandler(PetFileHandler::getInstance()),
               currentSortStrategy(&ageAscending) {
        listOfPets = fileHandler->loadPets();
    }

    ~PetList() {
        fileHandler->savePets(listOfPets);
        for (auto pet : listOfPets) {
            delete pet;
        }
    }

    void setSortStrategy(SortStrategy* strategy) {
        if (strategy) {
            currentSortStrategy = strategy;
        }
    }

    vector<Pet *> &getAllPets() {
        return listOfPets;
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

        int choice = getInputInt("\nEnter the number of the pet to manage (0 to cancel): ", 0, pendingPets.size());

        if (choice == 0) {
            cout << "Approval cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = pendingPets[choice - 1];
        string petName = selectedPet->getName(); // Store name for message before potential deletion

        string decisionStr;
        decisionStr = getValidatedString("\nApprove (A) or Reject (R) this pet submission? (A/R): ");

        if (decisionStr.length() == 1) {
            char decision = tolower(decisionStr[0]);

            if (decision == 'a') {
                // Approve - make pet available for adoption
                selectedPet->setStatus("available");
                cout << "Pet '" << petName << "' approved and now available for adoption!" << endl;
                fileHandler->savePets(listOfPets);
            } else if (decision == 'r') {
                // Reject - delete the pet
                auto it = find(listOfPets.begin(), listOfPets.end(), selectedPet);
                if (it != listOfPets.end()) {
                    delete selectedPet; // Free memory
                    listOfPets.erase(it); // Remove from vector
                    cout << "Pet '" << petName << "' rejected and removed from the system." << endl;
                    fileHandler->savePets(listOfPets);
                }
            } else {
                cout << "Invalid input. No action taken." << endl;
            }
        } else {
            cout << "Invalid input. No action taken." << endl;
        }

        system("pause");
        system("cls");
    }

    void addPet(const Account *account) {
    string accountType = account->getAccountType();
    cout << "\t   Add A Pet" << endl << "(enter '0' at any time to cancel)" << endl << endl;

    // Name input with cancellation
    string name = getValidatedString("Name: ");
    if (name == "0") {
        cout << "\nPet addition cancelled." << endl;
        system("pause");
        system("cls");
        return;
    }

    // Age input with validation and cancellation
    float age = 0;
    while (true) {
        string ageInput = getValidatedString("Age: ");
        if (ageInput == "0") {
            cout << "\nPet addition cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        try {
            age = stof(ageInput);
            if (age <= 0) {
                cout << "Age must be positive. Please try again." << endl;
                continue;
            }
            break;
        } catch (const exception& e) {
            cout << "Invalid age entered. Please enter a valid positive number." << endl;
        }
    }

    // Pet type selection with cancellation
    string typeInput;
    while (true) {
        typeInput = getValidatedString("Type (A = Dog, B = Cat): ");
        if (typeInput == "0") {
            cout << "\nPet addition cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }
        
        if (typeInput == "a" || typeInput == "b") {
            break;
        }
        cout << "Invalid pet type. Please enter A or B." << endl;
    }

    // Breed input with cancellation
    string breed;
    if (typeInput == "a") {
        breed = getValidatedString("Dog's Breed: ");
    } else {
        breed = getValidatedString("Cat's Breed: ");
    }
    
    if (breed == "0") {
        cout << "\nPet addition cancelled." << endl;
        system("pause");
        system("cls");
        return;
    }

    // Determine status based on account type
    string status = (accountType == "RegularUser") ? "pending" : "available";
    string submittedBy = (accountType == "RegularUser") ? account->getUsername() : "Staff";

    // Create the appropriate pet type
    Pet* newPet = nullptr;
    if (typeInput == "a") {
        newPet = new Dog(name, age, "Dog", breed, status, submittedBy, "none");
    } else {
        newPet = new Cat(name, age, "Cat", breed, status, submittedBy, "none");
    }

    // Add to list and save
    listOfPets.push_back(newPet);
    fileHandler->savePets(listOfPets);

    // Show success message
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
        int choice = getInputInt("Enter the number of the pet to delete (0 to cancel): ", 0, listOfPets.size());

        if (choice == 0) {
            cout << "Action cancelled." << endl;
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

        string name = listOfPets[choice - 1]->getName();
        listOfPets.erase(listOfPets.begin() + (choice - 1));
        fileHandler->savePets(listOfPets); // Save changes
        cout << "Deleted Pet: " << name << endl;

        system("pause");
    }

    void viewAllPets(string type = "All") const {
        vector<Pet*> filteredPets;
        vector<Pet*> availablePets = getAvailablePets();

        copy_if(availablePets.begin(), availablePets.end(), back_inserter(filteredPets),
            [type](Pet* pet) { return type == "All" || pet->getType() == type; });

        string title = type == "All" ? "Available Pets" : "Available " + type + "s";
        displayPets(filteredPets, title);

        if (filteredPets.empty()) {
            cout << "No pets of this type available." << endl;
            system("pause");
        }
    }

    void viewPetsSortedByAge(bool ascending = true) {
        vector<Pet*> petsToSort = getAvailablePets();

        if (ascending) {
            ageAscending.sort(petsToSort);
        } else {
            ageDescending.sort(petsToSort);
        }

        string title = ascending ? "Pets Sorted by Age (Ascending)"
                                : "Pets Sorted by Age (Descending)";
        displayPets(petsToSort, title);
    }

    void viewAndAdoptPet(string type, Account *currentAccount) const {
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

        int choice = getInputInt("Enter the number of the pet to adopt (0 to cancel): ", 0, filteredPets.size());

        if (choice == 0) {
            cout << "Adoption cancelled." << endl;
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = filteredPets[choice - 1];

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

void filterPet(Account* currentAccount) {
    while (true) {
        // Display main filter menu
        system("cls");
        vector<Pet*> availablePets = getAvailablePets();
        viewAllPets("All");

        cout << "\nFilter Pets (enter pet number to adopt):\n"
             << "a) By Type\n"
             << "b) By Age\n"
             << "x) Return to menu\n";

        string input = getValidatedString("\nChoice: ");

        if (input == "x") {
            return; // Exit filtering
        }
        else if (input == "a") {
            handleTypeFilter(currentAccount);
        }
        else if (input == "b") {
            handleAgeFilter();
        }
        else {
            handleAdoptionInput(input, currentAccount);
        }
    }
}



    void viewAndEditPetInformation() {
        vector<Pet *> petsToEdit = getAllPets();

        if (petsToEdit.empty()) {
            cout << "No pets available for editing.\n";
            system("pause");
            return;
        }

        int choice = 0;

        system("cls");
        cout << string(80, '-') << endl;
        cout << "\t\tEdit Pets" << endl;
        cout << string(80, '-') << endl;

        cout << setw(5) << left << "No."
                << setw(20) << left << "Name"
                << setw(10) << left << "Age"
                << setw(15) << left << "Type"
                << setw(25) << left << "Breed" << endl;
        cout << string(80, '-') << endl;

        for (int i = 0; i < petsToEdit.size(); ++i) {
            cout << setw(5) << left << i + 1
                    << setw(20) << left << petsToEdit[i]->getName()
                    << setw(10) << left << petsToEdit[i]->getAge()
                    << setw(15) << left << petsToEdit[i]->getType()
                    << setw(25) << left << getBreed(petsToEdit[i]) << endl;
        }

        choice = getInputInt("\nEnter the number of the pet to edit (0 to cancel): ", 0, petsToEdit.size());

        if (choice == 0) {
            cout << "Edit cancelled.\n";
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = petsToEdit[choice - 1];

        system("cls");
        cout << "\nEditing Pet: " << selectedPet->getName() << endl;

        string newName;
        string newAge;
        string newType;
        string newBreed;

        // Name edit
        cout << "Current Name: " << selectedPet->getName() << "\nEnter new name (or press Enter to keep): ";
        getline(cin, newName);
        if (editInformation(selectedPet->getName(), newName)) {
            selectedPet->setName(newName);
        }

        // Age edit
        cout << "Current Age: " << selectedPet->getAge() << "\nEnter new age (or press Enter to keep): ";
        getline(cin, newAge);
        if (!newAge.empty()) {
            try {
                float newAgeF = stof(newAge);
                if (newAgeF > 0) selectedPet->setAge(newAgeF);
                else cout << "Invalid age. Must be positive. Keeping previous age.\n";
            } catch (...) {
                cout << "Invalid age format. Keeping previous age.\n";
            }
        }

        // Type edit - simplified
        cout << "Current Type: " << selectedPet->getType()
                << "\nEnter new type (Dog/Cat) (or press Enter to keep): ";
        getline(cin, newType);
        if (!newType.empty()) {
            // Convert to lowercase for comparison
            transform(newType.begin(), newType.end(), newType.begin(), ::tolower);

            if (newType == "cat" || newType == "dog") {
                // Capitalize first letter for consistency
                newType[0] = toupper(newType[0]);
                selectedPet->setType(newType);
            } else {
                cout << "Invalid type. Must be 'Dog' or 'Cat'. Keeping previous type.\n";
            }
        }

        // Breed edit
        cout << "Current Breed: " << getBreed(selectedPet)
                << "\nEnter new breed (or press Enter to keep): ";
        getline(cin, newBreed);
        if (editInformation(getBreed(selectedPet), newBreed)) {
            setBreed(selectedPet, newBreed);
        }

        fileHandler->savePets(listOfPets); // Save updated list

        cout << "\nPet updated successfully.\n";
        selectedPet->viewPet();
        cout << endl;
        system("pause");
        system("cls");
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
        int choice = getInputInt("\nEnter the number of the pet to manage (0 to cancel): ", 0, pendingPets.size());

        if (choice == 0) {
            cout << "Adoption cancelled ." << endl;
            system("pause");
            system("cls");
            return;
        }

        Pet *selectedPet = pendingPets[choice - 1];

        // Handle approval/rejection input
        string decisionStr;
        decisionStr = getValidatedString("\nApprove (A) or Reject (R) this adoption request? (A/R): ");

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


// Static member definitions
AgeAscendingSort PetList::ageAscending;
AgeDescendingSort PetList::ageDescending;

int main() {
    /* To Do List:
        - FlowChart
        - Class Diagram
        - Use Case
        Code:
            Check for Errors
            Multiple Strategy
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
        Account *currentAccount = nullptr;
        cout << "Pet Adoption System" << endl;
        cout << "a) Register" << endl;
        cout << "b) Login" << endl;
        cout << "c) Exit" << endl << endl;
        choice = getValidatedString("Choice: ");

        system("cls");
        if (choice == "a") {
            registerAccount.addAccount("RegularUser");
        } else if (choice == "b") {
            currentAccount = logInAccount.logIn(registerAccount);
            if (currentAccount == nullptr) {
                continue; // This returns to the main menu
            }
            system("cls");
            if (currentAccount->getAccountType() == "Admin") {
                adminAccount.adminFunctions(registerAccount);
            } else if (currentAccount->getAccountType() == "Staff") {
                do {
                    staffAccount.userInterface();
                    choice = getValidatedString("Choice: ");
                    system("cls");
                    if (choice == "a") {
                        //Call animal class function - add pet using the credentials of the current account
                        animal.addPet(currentAccount);
                    } else if (choice == "b") {
                        //Call animal class function to delete the pet
                        animal.deletePet();
                    } else if (choice == "c") {
                        //Call animal class function to view and approve the adoption requests
                        animal.viewAndApproveAdoptions();
                    } else if (choice == "d") {
                        //Animal class function to view and approve pets that the users put up for adoption
                        animal.viewAndApprovePets();
                    } else if (choice == "e") {
                        //Call animal class function to view all pets
                        animal.viewAllPets("All");
                        cout << endl;
                        system("pause");
                    } else if (choice == "g") {
                        //Animal class function to view and approve pets that the users put up for adoption
                        animal.viewAndEditPetInformation();
                    } else if (choice == "x") {
                        //If the user inputs x, they will log out of the account
                        cout << "Logging Out" << endl;
                        system("pause");
                        system("cls");
                        break;
                    }
                } while (choice != "x"); //Loop the entire process of staff menu until the user logs out
            } else if (currentAccount->getAccountType() == "RegularUser") {
                do {
                    userAccount.userInterface();
                    choice = getValidatedString("Choice: ");
                    system("cls");

                    if (choice == "a") {
                        // Adopt a Pet (View + Adopt)
                        animal.filterPet(currentAccount);
                    } else if (choice == "b") {
                        // Add Pet (requires staff approval)
                        animal.addPet(currentAccount);
                    } else if (choice == "c") {
                        // view all pets (no adoption)
                        animal.viewAllPets("All");
                        cout << endl;
                        system("pause");
                    } else if (choice == "d") {
                        // View pets added by user
                        animal.viewPetsAddedByUser(currentAccount);
                    } else if (choice == "e") {
                        // View adopted pets
                        animal.viewPetsAdoptedByUser(currentAccount);
                    } else if (choice == "f") {
                        // Edit account info
                        userAccount.editPersonalInformation(currentAccount, accounts, PetFileHandler::getInstance());
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
