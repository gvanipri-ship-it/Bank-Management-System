#include "Bank.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

Bank::Bank(const std::string& dataFilePath)
    : dataFile(dataFilePath), nextAccountNumber(1001) {
    loadAccounts();
}

// ─── File I/O ─────────────────────────────────────────────────────────────────

void Bank::loadAccounts() {
    std::ifstream file(dataFile);
    if (!file.is_open()) return;   // first run — no file yet

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            Account acc = Account::deserialize(line);
            accounts.push_back(acc);
            if (acc.getAccountNumber() >= nextAccountNumber)
                nextAccountNumber = acc.getAccountNumber() + 1;
        } catch (...) {
            // skip malformed lines
        }
    }
}

void Bank::saveAccounts() const {
    // Ensure the data directory exists by attempting to open the file
    std::ofstream file(dataFile, std::ios::trunc);
    if (!file.is_open()) {
        Utils::printError("Could not write to data file: " + dataFile);
        return;
    }
    for (const auto& acc : accounts)
        file << acc.serialize() << "\n";
}

// ─── Internal helpers ─────────────────────────────────────────────────────────

int Bank::generateAccountNumber() {
    return nextAccountNumber++;
}

Account* Bank::findAccount(int accNo) {
    for (auto& acc : accounts)
        if (acc.getAccountNumber() == accNo)
            return &acc;
    return nullptr;
}

// ─── Account management ───────────────────────────────────────────────────────

int Bank::createAccount(const std::string& name, const std::string& pin,
                        AccountType type, double initialDeposit) {
    if (!Utils::isValidName(name)) {
        Utils::printError("Invalid name. Use letters and spaces only.");
        return -1;
    }
    if (!Utils::isValidPin(pin)) {
        Utils::printError("PIN must be exactly 4 digits.");
        return -1;
    }
    if (initialDeposit < 0) {
        Utils::printError("Initial deposit cannot be negative.");
        return -1;
    }

    int accNo = generateAccountNumber();
    accounts.emplace_back(accNo, name, pin, type, initialDeposit);
    saveAccounts();
    return accNo;
}

bool Bank::deleteAccount(int accNo, const std::string& pin) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [&](const Account& a){ return a.getAccountNumber() == accNo; });

    if (it == accounts.end()) {
        Utils::printError("Account not found.");
        return false;
    }
    if (!it->verifyPin(pin)) {
        Utils::printError("Incorrect PIN.");
        return false;
    }

    accounts.erase(it);
    saveAccounts();
    return true;
}

// ─── Transactions ─────────────────────────────────────────────────────────────

bool Bank::deposit(int accNo, double amount) {
    Account* acc = findAccount(accNo);
    if (!acc) { Utils::printError("Account not found."); return false; }

    if (!acc->deposit(amount)) {
        Utils::printError("Invalid deposit amount.");
        return false;
    }
    saveAccounts();
    return true;
}

bool Bank::withdraw(int accNo, const std::string& pin, double amount) {
    Account* acc = findAccount(accNo);
    if (!acc) { Utils::printError("Account not found."); return false; }

    if (!acc->verifyPin(pin)) { Utils::printError("Incorrect PIN."); return false; }

    if (!acc->withdraw(amount)) {
        Utils::printError("Insufficient funds or invalid amount.");
        return false;
    }
    saveAccounts();
    return true;
}

bool Bank::transfer(int fromAccNo, const std::string& pin,
                    int toAccNo, double amount) {
    Account* from = findAccount(fromAccNo);
    Account* to   = findAccount(toAccNo);

    if (!from) { Utils::printError("Source account not found.");      return false; }
    if (!to)   { Utils::printError("Destination account not found."); return false; }
    if (!from->verifyPin(pin)) { Utils::printError("Incorrect PIN."); return false; }

    if (!from->withdraw(amount)) {
        Utils::printError("Insufficient funds or invalid amount.");
        return false;
    }
    to->deposit(amount);
    saveAccounts();
    return true;
}

// ─── Queries ──────────────────────────────────────────────────────────────────

void Bank::checkBalance(int accNo, const std::string& pin) const {
    const Account* acc = nullptr;
    for (const auto& a : accounts)
        if (a.getAccountNumber() == accNo) { acc = &a; break; }

    if (!acc) { Utils::printError("Account not found."); return; }
    if (!acc->verifyPin(pin)) { Utils::printError("Incorrect PIN."); return; }

    std::cout << "\n  Balance for account " << accNo
              << " (" << acc->getHolderName() << "): $"
              << std::fixed << std::setprecision(2) << acc->getBalance() << "\n\n";
}

void Bank::displayAccount(int accNo) const {
    for (const auto& a : accounts)
        if (a.getAccountNumber() == accNo) { a.displayDetails(); return; }
    Utils::printError("Account not found.");
}

void Bank::displayAllAccounts() const {
    if (accounts.empty()) {
        Utils::printInfo("No accounts found.");
        return;
    }
    Utils::printSeparator();
    std::cout << std::left
              << "  " << std::setw(12) << "Acc. No."
              << std::setw(25) << "Holder Name"
              << std::setw(12) << "Type"
              << "Balance\n";
    Utils::printSeparator();
    for (const auto& acc : accounts) {
        std::cout << "  ";
        acc.displaySummary();
    }
    Utils::printSeparator();
}

void Bank::searchByName(const std::string& name) const {
    bool found = false;
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    for (const auto& acc : accounts) {
        std::string accName = acc.getHolderName();
        std::transform(accName.begin(), accName.end(), accName.begin(), ::tolower);
        if (accName.find(lower) != std::string::npos) {
            acc.displayDetails();
            found = true;
        }
    }
    if (!found) Utils::printInfo("No accounts found matching \"" + name + "\".");
}
