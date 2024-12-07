#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype> // For character checking
#include "project.h"
#include <algorithm>

using namespace std;

// Account class methods implementation
Account::Account(int accountNumber, const string& name, const string& phone, const string& password, double balance, bool hasATM, int atmCardNumber, int atmPin)
    : accountNumber(accountNumber), name(name), phone(phone), password(password), balance(balance), hasATM(hasATM), atmCardNumber(atmCardNumber), atmPin(atmPin) {}

int Account::getAccountNumber() const { return accountNumber; }
double Account::getBalance() const { return balance; }
string Account::getName() const { return name; }
string Account::getPhone() const { return phone; }
bool Account::verifyPassword(const string& inputPassword) const { return inputPassword == password; }

void Account::deposit(double amount) {
    if (amount <= 0) {
        throw invalid_argument("Invalid deposit amount.");
    }
    balance += amount;
    cout << "Deposited: " << amount << ", New Balance: " << balance << "\n";
}

void Account::withdraw(double amount) {
    if (amount <= 0) {
        throw invalid_argument("Invalid withdrawal amount.");
    }
    if (amount > balance) {
        throw runtime_error("Insufficient funds for withdrawal.");
    }
    balance -= amount;
    cout << "Withdrawn: " << amount << ", New Balance: " << balance << "\n";
}

void Account::displayAccountInfo() const {
    cout << "Account Number: " << accountNumber << "\n"
         << "Name: " << name << "\n"
         << "Phone: " << phone << "\n"
         << "Balance: " << balance << "\n";
    if (hasATM) {
        cout << "ATM Card Number: " << atmCardNumber << "\n";
    } else {
        cout << "ATM Card: No\n";
    }
}

ostream& operator<<(ostream& os, const Account& account) {
    os << account.accountNumber << "," << account.name << "," << account.phone << ","
       << account.password << "," << account.balance << "," << account.hasATM << ","
       << account.atmCardNumber << "," << account.atmPin;
    return os;
}

istream& operator>>(istream& is, Account& account) {
    is >> account.accountNumber;
    is.ignore(); // Ignore comma
    getline(is, account.name, ',');
    getline(is, account.phone, ',');
    getline(is, account.password, ',');
    is >> account.balance;
    is.ignore(); // Ignore comma
    is >> account.hasATM;
    is.ignore(); // Ignore comma
    is >> account.atmCardNumber;
    is.ignore(); // Ignore comma
    is >> account.atmPin;
    return is;
}

Account& Account::operator=(const Account& account) {
    if (this != &account) {
        accountNumber = account.accountNumber;
        name = account.name;
        phone = account.phone;
        password = account.password;
        balance = account.balance;
        hasATM = account.hasATM;
        atmCardNumber = account.atmCardNumber;
        atmPin = account.atmPin;
    }
    return *this;
}

// AccountManager class methods implementation
AccountManager::AccountManager() : accountCount(0) {}

AccountManager::~AccountManager() {}

void AccountManager::createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        throw runtime_error("Cannot create more accounts. Maximum limit reached.");
    }

    string phone;
    cout << "Enter your phone number (10 digits only): ";
    cin >> phone;
    if (phone.length() != 10 || !isValidNumber(phone)) {
        throw invalid_argument("Invalid phone number. It must be exactly 10 digits.");
    }

    // Check if the mobile number already exists
    if (phoneExists(phone)) {
        throw invalid_argument("An account with this mobile number already exists.");
    }

    string name, password;
    cout << "Enter your name: ";
    cin.ignore(); // To ignore any newline character left in the buffer
    getline(cin, name); // Use getline to read the full name including spaces
    cout << "Set your password (must contain at least one uppercase letter and one special character): ";
    cin >> password;

    // Password validation
    bool hasUpper = false, hasSpecial = false;
    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        if (!isalnum(ch)) hasSpecial = true;
    }
    if (!hasUpper || !hasSpecial) {
        throw invalid_argument("Password must contain at least one uppercase letter and one special character.");
    }

    bool hasATM = false;
    int atmCardNumber = 0;
    int atmPin = 0;
    cout << "Do you want an ATM card? (yes/no): ";
    string response;
    cin >> response;
    if (response == "yes") {
        hasATM = true;
        atmCardNumber = 10000 + rand() % 90000; // Simple random ATM card number
        atmPin = 1234; // Simple fixed ATM pin for demonstration
        cout << "Your ATM Card Number is: " << atmCardNumber << "\n";
    }

    int newAccountNumber = generateNewAccountNumber();
    accounts[accountCount++] = Account(newAccountNumber, name, phone, password, 0.0, hasATM, atmCardNumber, atmPin);
    cout << "Account created successfully. Your account number is: " << newAccountNumber << "\n";
}

void AccountManager::accessAccount() {
    int accountNumber;
    cout << "Enter your account number: ";
    cin >> accountNumber;

    string password;
    cout << "Enter your password: ";
    cin >> password;

    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getAccountNumber() == accountNumber && accounts[i].verifyPassword(password)) {
            cout << "Access granted.\n";
            int actionChoice;
            do {
                cout << "\nAccount Menu:\n";
                cout << "1. View Balance\n";
                cout << "2. Deposit\n";
                cout << "3. Withdraw\n";
                cout << "4. Transfer Money\n"; // New option for transferring money
                cout << "5. Exit Account Menu\n";
                cout << "Enter your choice: ";
                cin >> actionChoice;

                try {
                    switch (actionChoice) {
                        case 1:
                            cout << "Current Balance: " << accounts[i].getBalance() << "\n";
                            break;
                        case 2: {
                            double amount;
                            cout << "Enter amount to deposit: ";
                            cin >> amount;
                            accounts[i].deposit(amount);
                            break;
                        }
                        case 3: {
                            double amount;
                            cout << "Enter amount to withdraw: ";
                            cin >> amount;
                            accounts[i].withdraw(amount);
                            break;
                        }
                        case 4: { // Transfer Money
                            transferMoney(accounts[i].getAccountNumber());
                            break;
                        }
                        case 5:
                            cout << "Exiting account menu.\n";
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                    }
                } catch (const invalid_argument& e) {
                    cout << "Error: " << e.what() << endl;
                } catch (const runtime_error& e) {
                    cout << "Error: " << e.what() << endl;
                }
            } while (actionChoice != 5);
            return;
        }
    }
    cout << "Invalid account number or password.\n";
}

void AccountManager::deleteAccount(int accountNumber) {
    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getAccountNumber() == accountNumber) {
            for (int j = i; j < accountCount - 1; ++j) {
                accounts[j] = accounts[j + 1];
            }
            --accountCount;
            cout << "Account deleted successfully.\n";
            return;
        }
    }
    cout << "Account not found.\n";
}

void AccountManager::displayAllAccounts() const {
    cout << "\nAll Accounts:\n";
    for (int i = 0; i < accountCount; ++i) {
        accounts[i].displayAccountInfo();
        cout << "--------------------------------\n";
    }
}

void AccountManager::applyInterest(double rate) {
    for (int i = 0; i < accountCount; ++i) {
        double interest = accounts[i].getBalance() * (rate / 100);
        accounts[i].deposit(interest);
    }
    cout << "Interest applied to all accounts.\n";
}

void AccountManager::applyServiceCharge(double charge) {
    for (int i = 0; i < accountCount; ++i) {
        try {
            accounts[i].withdraw(charge);
        } catch (const runtime_error& e) {
            cout << "Error for account " << accounts[i].getAccountNumber() << ": " << e.what() << endl;
        }
    }
    cout << "Service charge applied to all accounts.\n";
}

void AccountManager::transferMoney(int senderAccountNumber) {
    double amount;
    int recipientAccountNumber;
    
    cout << "Enter the recipient's account number: ";
    cin >> recipientAccountNumber;
    
    cout << "Enter amount to transfer: ";
    cin >> amount;

    if (amount <= 0) {
        throw invalid_argument("Transfer amount must be positive.");
    }

    auto senderIt = find_if(accounts, accounts + accountCount, [&](const Account& acc) { return acc.getAccountNumber() == senderAccountNumber; });
    auto recipientIt = find_if(accounts, accounts + accountCount, [&](const Account& acc) { return acc.getAccountNumber() == recipientAccountNumber; });

    if (senderIt != accounts + accountCount && recipientIt != accounts + accountCount) {
        if (senderIt->getBalance() >= amount) {
            senderIt->withdraw(amount);
            recipientIt->deposit(amount);
            cout << "Transfer successful! " << amount << " transferred to Account Number: " << recipientAccountNumber << "\n";
        } else {
            throw runtime_error("Insufficient funds for transfer.");
        }
    } else {
        cout << "One of the account numbers is invalid.\n";
    }
}

int AccountManager::generateNewAccountNumber() {
    return accountCount + 1000; // Simple new account number generation
}

bool AccountManager::isValidNumber(const string& str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

// Admin class methods implementation
Admin::Admin() : username(""), password("") {}

bool Admin::isRegistered() const {
    return !username.empty();
}

void Admin::registerAdmin(const string& user, const string& pass) {
    username = user;
    password = pass;
    cout << "Admin registered successfully.\n";
}

bool Admin::verifyCredentials(const string& user, const string& pass) const {
    return user == username && pass == password;
}

// New function to display admin information
void Admin::displayAdminInfo() const {
    cout << "Admin Username: " << username << "\n";
}
bool AccountManager::phoneExists(const string& phone) {
    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getPhone() == phone) {
            return true; // Phone number already exists
        }
    }
    return false; // Phone number does not exist
}

// Admin login function
bool adminLogin(Admin& admin) {
    string username, password;

    if (!admin.isRegistered()) {
        cout << "No admin found. Please register.\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password (must contain at least one uppercase letter and one special character): ";
        cin >> password;

        // Password validation
        bool hasUpper = false, hasSpecial = false;
        for (char ch : password) {
            if (isupper(ch)) hasUpper = true;
            if (!isalnum(ch)) hasSpecial = true;
        }
        if (!hasUpper || !hasSpecial) {
            cout << "Error: Password must contain at least one uppercase letter and one special character.\n";
            return false;
        }

        admin.registerAdmin(username, password);
        return true; // Registration successful
    }

    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    return admin.verifyCredentials(username, password);
}