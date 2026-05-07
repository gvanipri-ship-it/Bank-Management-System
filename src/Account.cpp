#include "Account.h"
#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

Account::Account(int accNo, const std::string& name, const std::string& pin,
                 AccountType t, double initialBalance)
    : accountNumber(accNo), holderName(name),
      pin(Utils::hashPin(pin)), type(t), balance(initialBalance) {}

// ─── Authentication ───────────────────────────────────────────────────────────

bool Account::verifyPin(const std::string& inputPin) const {
    return pin == Utils::hashPin(inputPin);
}

// ─── Operations ──────────────────────────────────────────────────────────────

bool Account::deposit(double amount) {
    if (!Utils::isValidAmount(amount)) return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (!Utils::isValidAmount(amount)) return false;
    if (amount > balance) return false;
    balance -= amount;
    return true;
}

// ─── Serialization ───────────────────────────────────────────────────────────

std::string Account::serialize() const {
    // Format: accNo|holderName|hashedPin|type|balance
    std::ostringstream oss;
    oss << accountNumber << "|"
        << holderName    << "|"
        << pin           << "|"
        << accountTypeToString(type) << "|"
        << std::fixed << std::setprecision(2) << balance;
    return oss.str();
}

Account Account::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    Account acc;

    std::getline(iss, token, '|'); acc.accountNumber = std::stoi(token);
    std::getline(iss, acc.holderName, '|');
    std::getline(iss, acc.pin,        '|');
    std::getline(iss, token,          '|'); acc.type = stringToAccountType(token);
    std::getline(iss, token,          '|'); acc.balance = std::stod(token);

    return acc;
}

// ─── Display ─────────────────────────────────────────────────────────────────

void Account::displaySummary() const {
    std::cout << std::left
              << std::setw(12) << accountNumber
              << std::setw(25) << holderName
              << std::setw(12) << accountTypeToString(type)
              << "$" << std::fixed << std::setprecision(2) << balance
              << "\n";
}

void Account::displayDetails() const {
    Utils::printSeparator();
    std::cout << "  Account Number : " << accountNumber                  << "\n"
              << "  Holder Name    : " << holderName                     << "\n"
              << "  Account Type   : " << accountTypeToString(type)      << "\n"
              << "  Balance        : $" << std::fixed << std::setprecision(2)
                                        << balance                       << "\n";
    Utils::printSeparator();
}

// ─── Utility ─────────────────────────────────────────────────────────────────

std::string Account::accountTypeToString(AccountType t) {
    return (t == AccountType::SAVINGS) ? "SAVINGS" : "CHECKING";
}

AccountType Account::stringToAccountType(const std::string& s) {
    return (s == "SAVINGS") ? AccountType::SAVINGS : AccountType::CHECKING;
}
