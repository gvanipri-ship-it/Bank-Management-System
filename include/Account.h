#pragma once
#include <string>

enum class AccountType {
    SAVINGS,
    CHECKING
};

class Account {
private:
    int         accountNumber;
    std::string holderName;
    std::string pin;          // stored as plain string; hashed in a real system
    AccountType type;
    double      balance;

public:
    // Constructors
    Account() = default;
    Account(int accNo, const std::string& name, const std::string& pin,
            AccountType type, double initialBalance = 0.0);

    // Getters
    int                getAccountNumber() const { return accountNumber; }
    const std::string& getHolderName()    const { return holderName;    }
    AccountType        getType()          const { return type;          }
    double             getBalance()       const { return balance;       }

    // Authentication
    bool verifyPin(const std::string& inputPin) const;

    // Operations
    bool deposit(double amount);
    bool withdraw(double amount);

    // Serialization helpers (CSV-style)
    std::string serialize()   const;
    static Account deserialize(const std::string& line);

    // Display
    void displaySummary() const;
    void displayDetails() const;

    // Utility
    static std::string accountTypeToString(AccountType t);
    static AccountType stringToAccountType(const std::string& s);
};
