#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept> // For exception handling
#include "project.h"

using namespace std;

bool adminLogin(Admin& admin);

int main() {
    AccountManager manager;
    Admin admin;
    int choice;

    do {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create Account\n";
        cout << "2. Access Account\n";
        cout << "3. Delete Account\n";
        cout << "4. Admin Login\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1:
                    manager.createAccount();
                    break;
                case 2:
                    manager.accessAccount();
                    break;
                case 3: {
                    int accountNumber;
                    cout << "Enter your account number to delete: ";
                    cin >> accountNumber;
                    manager.deleteAccount(accountNumber);
                    break;
                }
                case 4:{
                    if (adminLogin(admin)) {
                        cout << "Admin access granted.\n";
                        int adminChoice;
                        do {
                            cout << "\nAdmin Menu:\n";
                            cout << "1. View all customer details\n";
                            cout << "2. Delete an account\n";
                            cout << "3. Apply Interest\n";
                            cout << "4. Apply Service Charge\n";
                            cout << "5. Display Admin Info\n"; // New option
                            cout << "6. Exit Admin Menu\n";
                            cout << "Enter your choice: ";
                            cin >> adminChoice;

                            switch (adminChoice) {
                                case 1:
                                    manager.displayAllAccounts();
                                    break;
                                case 2: {
                                    int accountNumber;
                                    cout << "Enter account number to delete: ";
                                    cin >> accountNumber;
                                    manager.deleteAccount(accountNumber);
                                    break;
                                }
                                case 3: {
                                    double interestRate;
                                    cout << "Enter interest rate (in percentage): ";
                                    cin >> interestRate;
                                    manager.applyInterest(interestRate);
                                    break;
                                }
                                case 4: {
                                    double serviceCharge;
                                    cout << "Enter service charge amount: ";
                                    cin >> serviceCharge;
                                    manager.applyServiceCharge(serviceCharge);
                                    break;
                                }
                                case 5:
                                    admin.displayAdminInfo();
                                    break;
                                case 6:
                                    cout << "Exiting admin menu.\n";
                                    break;
                                default:
                                    cout << "Invalid choice. Please try again.\n";
                            }
                        } while (adminChoice != 6);
                    } else {
                        cout << "Admin login failed. Access denied.\n";
                    }
                    break;
                }
                case 5:
                    cout << "Thank you for using the Banking System! Have a great day ahead!";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 5);

    return 0;
}