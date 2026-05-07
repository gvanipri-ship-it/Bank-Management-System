#pragma once
#include "Account.h"
#include <vector>
#include <string>
#include <optional>

class Bank {
private:
    std::string              dataFile;
    std::vector<Account>     accounts;
    int                      nextAccountNumber;

    // File I/O
    void loadAccounts();
    void saveAccounts() const;

    // Internal helpers
    int  generateAccountNumber();
    Account* findAccount(int accNo);

public:
    explicit Bank(const std::string& dataFilePath = "data/accounts.dat");

    // Account management
    int  createAccount(const std::string& name, const std::string& pin,
                       AccountType type, double initialDeposit = 0.0);
    bool deleteAccount(int accNo, const std::string& pin);

    // Transactions
    bool deposit (int accNo, double amount);
    bool withdraw(int accNo, const std::string& pin, double amount);
    bool transfer(int fromAccNo, const std::string& pin,
                  int toAccNo,   double amount);

    // Queries
    void checkBalance(int accNo, const std::string& pin) const;
    void displayAccount(int accNo) const;
    void displayAllAccounts() const;

    // Search
    void searchByName(const std::string& name) const;
};
