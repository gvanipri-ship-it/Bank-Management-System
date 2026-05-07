#pragma once
#include <string>

namespace Utils {
    // Secure PIN input (masks characters)
    std::string getHiddenInput(const std::string& prompt);

    // Input helpers
    int    getIntInput   (const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);

    // Validation
    bool isValidPin   (const std::string& pin);
    bool isValidName  (const std::string& name);
    bool isValidAmount(double amount);

    // Display helpers
    void printHeader(const std::string& title);
    void printSeparator();
    void printSuccess(const std::string& msg);
    void printError  (const std::string& msg);
    void printInfo   (const std::string& msg);

    // Simple hash (djb2) — not cryptographic, but better than plain text
    std::string hashPin(const std::string& pin);
}
