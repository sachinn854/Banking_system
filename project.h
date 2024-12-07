#include <iostream>
#include <string>
#include <stdexcept> // For exception handling

using namespace std;

// Account class declaration
class Account {
public:
    Account(int accountNumber = 0, const string& name = "", const string& phone = "", const string& password = "", double balance = 0.0, bool hasATM = false, int atmCardNumber = 0, int atmPin = 0);
    
    int getAccountNumber() const;
    double getBalance() const;
    string getName() const;
    string getPhone() const;
    bool verifyPassword(const string& inputPassword) const;

    void deposit(double amount);
    void withdraw(double amount);
    void displayAccountInfo() const;

    // Operator Overloading
    friend ostream& operator<<(ostream& os, const Account& account);
    friend istream& operator>>(istream& is, Account& account);
    
    Account& operator=(const Account& account); // Assignment operator

private:
    int accountNumber;
    string name;
    string phone;
    string password;
    double balance;
    bool hasATM;
    int atmCardNumber;
    int atmPin;
};

// Abstract class for Account Management
class IAccountManager {
public:
    virtual void createAccount() = 0;
    virtual void accessAccount() = 0;
    virtual void deleteAccount(int accountNumber) = 0;
    virtual void displayAllAccounts() const = 0;
    virtual void applyInterest(double rate) = 0;
    virtual void applyServiceCharge(double charge) = 0;
    virtual void transferMoney(int senderAccountNumber) = 0;
    virtual ~IAccountManager() {} // Virtual destructor
};

// AccountManager class declaration
class AccountManager : public IAccountManager {
public:
    AccountManager();
    ~AccountManager();
    bool phoneExists(const string&);
    void createAccount() override;
    void accessAccount() override;
    void deleteAccount(int accountNumber) override;
    void displayAllAccounts() const override;
    void applyInterest(double rate) override;
    void applyServiceCharge(double charge) override;
    void transferMoney(int senderAccountNumber) override; // Implemented as per previous code

private:
    static const int MAX_ACCOUNTS = 100;
    Account accounts[MAX_ACCOUNTS];
    int accountCount;

    int generateNewAccountNumber();
    bool isValidNumber(const string& str);
};

// Abstract class for Admin management
class AbstractAdmin {
public:
    virtual bool isRegistered() const = 0;
    virtual void registerAdmin(const string& user, const string& pass) = 0;
    virtual bool verifyCredentials(const string& user, const string& pass) const = 0;
    virtual void displayAdminInfo() const = 0; // New virtual function
};

// Admin class declaration
class Admin : public AbstractAdmin {
public:
    Admin();
    bool isRegistered() const override;
    void registerAdmin(const string& user, const string& pass) override;
    bool verifyCredentials(const string& user, const string& pass) const override;
    void displayAdminInfo() const override; // Implement this function

private:
    string username;
    string password;
};