#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Account {
private:
    int accountID;
    string name;
    double balance;

public:
    Account() {}
    Account(int id, string n, double b) : accountID(id), name(n), balance(b) {}

    int getAccountID() const { return accountID; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount <= 0)
            throw invalid_argument("Deposit amount must be positive.");
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount > balance)
            throw invalid_argument("Insufficient balance.");
        balance -= amount;
    }

    void display() const {
        cout << left << setw(10) << accountID 
             << setw(20) << name 
             << "₹" << fixed << setprecision(2) << balance << endl;
    }

    // Serialize account to file
    void save(ofstream &outFile) const {
        outFile << accountID << " " << name << " " << balance << endl;
    }

    // Deserialize account from file
    bool load(ifstream &inFile) {
        inFile >> accountID >> name >> balance;
        return !inFile.eof();
    }
};

// Utility functions
void saveAccounts(const vector<Account> &accounts) {
    ofstream outFile("accounts.txt");
    for (const auto &acc : accounts)
        acc.save(outFile);
    outFile.close();
}

vector<Account> loadAccounts() {
    vector<Account> accounts;
    ifstream inFile("accounts.txt");
    if (!inFile)
        return accounts;

    Account acc;
    while (acc.load(inFile))
        accounts.push_back(acc);
    inFile.close();
    return accounts;
}

int generateAccountID(const vector<Account> &accounts) {
    return accounts.empty() ? 101 : accounts.back().getAccountID() + 1;
}

void viewAllAccounts(const vector<Account> &accounts) {
    cout << "\n--- All Accounts ---\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << "Balance\n";
    cout << "------------------------------------------\n";
    for (const auto &acc : accounts)
        acc.display();
}

int main() {
    vector<Account> accounts = loadAccounts();
    int choice;

    cout << "Welcome to Banking Management System\n";

    do {
        cout << "\n1. Create New Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Check Balance\n";
        cout << "5. View All Accounts\n";
        cout << "6. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        try {
            if (choice == 1) {
                string name;
                double deposit;
                cout << "Enter Name: ";
                cin >> name;
                cout << "Enter Initial Deposit: ";
                cin >> deposit;

                int id = generateAccountID(accounts);
                accounts.emplace_back(id, name, deposit);
                saveAccounts(accounts);
                cout << "Account Created Successfully! Account ID: " << id << endl;

            } else if (choice == 2) {
                int id;
                double amount;
                cout << "Enter Account ID: ";
                cin >> id;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;

                bool found = false;
                for (auto &acc : accounts) {
                    if (acc.getAccountID() == id) {
                        acc.deposit(amount);
                        saveAccounts(accounts);
                        cout << "Amount Deposited Successfully! New Balance: ₹" << acc.getBalance() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";

            } else if (choice == 3) {
                int id;
                double amount;
                cout << "Enter Account ID: ";
                cin >> id;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;

                bool found = false;
                for (auto &acc : accounts) {
                    if (acc.getAccountID() == id) {
                        acc.withdraw(amount);
                        saveAccounts(accounts);
                        cout << "Amount Withdrawn Successfully! New Balance: ₹" << acc.getBalance() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";

            } else if (choice == 4) {
                int id;
                cout << "Enter Account ID: ";
                cin >> id;

                bool found = false;
                for (const auto &acc : accounts) {
                    if (acc.getAccountID() == id) {
                        cout << "Account Holder: " << acc.getName() << "\nBalance: ₹" << acc.getBalance() << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";

            } else if (choice == 5) {
                viewAllAccounts(accounts);
            } else if (choice == 6) {
                cout << "Thank you for using the Banking Management System!\n";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }

    } while (choice != 6);

    return 0;
}
