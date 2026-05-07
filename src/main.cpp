#include "Bank.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <cstdlib>   // system()

// ─── Menu helpers ─────────────────────────────────────────────────────────────

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pauseScreen() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(1000, '\n');
}

// ─── Sub-menus ────────────────────────────────────────────────────────────────

static void menuCreateAccount(Bank& bank) {
    Utils::printHeader("CREATE NEW ACCOUNT");

    std::string name = Utils::getStringInput("  Full Name       : ");
    if (!Utils::isValidName(name)) {
        Utils::printError("Name must contain letters and spaces only.");
        pauseScreen(); return;
    }

    std::string pin = Utils::getHiddenInput("  Set 4-digit PIN : ");
    if (!Utils::isValidPin(pin)) {
        Utils::printError("PIN must be exactly 4 digits.");
        pauseScreen(); return;
    }

    std::cout << "\n  Account Type:\n"
              << "    1. Savings\n"
              << "    2. Checking\n";
    int typeChoice = Utils::getIntInput("  Choice          : ");
    AccountType type = (typeChoice == 2) ? AccountType::CHECKING : AccountType::SAVINGS;

    double initial = Utils::getDoubleInput("  Initial Deposit : $");

    int accNo = bank.createAccount(name, pin, type, initial);
    if (accNo != -1) {
        Utils::printSuccess("Account created successfully!");
        std::cout << "  Your Account Number: " << accNo << "\n"
                  << "  Keep it safe — you will need it for all transactions.\n\n";
    }
    pauseScreen();
}

static void menuDeposit(Bank& bank) {
    Utils::printHeader("DEPOSIT");
    int    accNo  = Utils::getIntInput   ("  Account Number : ");
    double amount = Utils::getDoubleInput("  Amount         : $");

    if (bank.deposit(accNo, amount))
        Utils::printSuccess("Deposit of $" + std::to_string(amount) + " successful.");
    pauseScreen();
}

static void menuWithdraw(Bank& bank) {
    Utils::printHeader("WITHDRAW");
    int    accNo  = Utils::getIntInput("  Account Number : ");
    std::string pin = Utils::getHiddenInput("  PIN            : ");
    double amount = Utils::getDoubleInput("  Amount         : $");

    if (bank.withdraw(accNo, pin, amount))
        Utils::printSuccess("Withdrawal of $" + std::to_string(amount) + " successful.");
    pauseScreen();
}

static void menuTransfer(Bank& bank) {
    Utils::printHeader("TRANSFER FUNDS");
    int    from   = Utils::getIntInput("  From Account   : ");
    std::string pin = Utils::getHiddenInput("  PIN            : ");
    int    to     = Utils::getIntInput("  To Account     : ");
    double amount = Utils::getDoubleInput("  Amount         : $");

    if (bank.transfer(from, pin, to, amount))
        Utils::printSuccess("Transfer of $" + std::to_string(amount) + " successful.");
    pauseScreen();
}

static void menuCheckBalance(Bank& bank) {
    Utils::printHeader("CHECK BALANCE");
    int accNo = Utils::getIntInput("  Account Number : ");
    std::string pin = Utils::getHiddenInput("  PIN            : ");
    bank.checkBalance(accNo, pin);
    pauseScreen();
}

static void menuViewAccount(Bank& bank) {
    Utils::printHeader("VIEW ACCOUNT DETAILS");
    int accNo = Utils::getIntInput("  Account Number : ");
    bank.displayAccount(accNo);
    pauseScreen();
}

static void menuDeleteAccount(Bank& bank) {
    Utils::printHeader("DELETE ACCOUNT");
    Utils::printInfo("Warning: This action is irreversible.");
    int accNo = Utils::getIntInput("  Account Number : ");
    std::string pin = Utils::getHiddenInput("  PIN            : ");

    if (bank.deleteAccount(accNo, pin))
        Utils::printSuccess("Account " + std::to_string(accNo) + " deleted.");
    pauseScreen();
}

static void menuSearch(Bank& bank) {
    Utils::printHeader("SEARCH ACCOUNTS");
    std::string name = Utils::getStringInput("  Search by name : ");
    bank.searchByName(name);
    pauseScreen();
}

// ─── Main menu ────────────────────────────────────────────────────────────────

static void showMainMenu() {
    Utils::printHeader("BANK MANAGEMENT SYSTEM");
    std::cout
        << "  1.  Create New Account\n"
        << "  2.  Deposit\n"
        << "  3.  Withdraw\n"
        << "  4.  Transfer Funds\n"
        << "  5.  Check Balance\n"
        << "  6.  View Account Details\n"
        << "  7.  List All Accounts\n"
        << "  8.  Search Account by Name\n"
        << "  9.  Delete Account\n"
        << "  0.  Exit\n";
    Utils::printSeparator();
}

// ─── Entry point ─────────────────────────────────────────────────────────────

int main() {
    // Ensure data directory exists (create it if missing)
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    system("mkdir -p data");
#endif

    Bank bank("data/accounts.dat");

    int choice = -1;
    while (choice != 0) {
        clearScreen();
        showMainMenu();
        choice = Utils::getIntInput("  Select option  : ");

        clearScreen();
        switch (choice) {
            case 1: menuCreateAccount(bank); break;
            case 2: menuDeposit      (bank); break;
            case 3: menuWithdraw     (bank); break;
            case 4: menuTransfer     (bank); break;
            case 5: menuCheckBalance (bank); break;
            case 6: menuViewAccount  (bank); break;
            case 7:
                Utils::printHeader("ALL ACCOUNTS");
                bank.displayAllAccounts();
                pauseScreen();
                break;
            case 8: menuSearch       (bank); break;
            case 9: menuDeleteAccount(bank); break;
            case 0:
                Utils::printInfo("Thank you for using Bank Management System. Goodbye!");
                break;
            default:
                Utils::printError("Invalid option. Please try again.");
                pauseScreen();
        }
    }
    return 0;
}
