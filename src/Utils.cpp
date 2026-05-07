#include "Utils.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
  #include <conio.h>   // _getch()
#else
  #include <termios.h>
  #include <unistd.h>
#endif

namespace Utils {

// ─── Secure PIN input ─────────────────────────────────────────────────────────

std::string getHiddenInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;

#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r') {   // Enter key
        if (ch == '\b') {               // Backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        } else {
            input += ch;
            std::cout << '*';
        }
    }
    std::cout << "\n";
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, input);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n";
#endif
    return input;
}

// ─── Input helpers ────────────────────────────────────────────────────────────

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid input. Please enter a whole number.");
    }
}

double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printError("Invalid input. Please enter a numeric value.");
    }
}

std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    // trim leading/trailing whitespace
    size_t start = value.find_first_not_of(" \t");
    size_t end   = value.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : value.substr(start, end - start + 1);
}

// ─── Validation ───────────────────────────────────────────────────────────────

bool isValidPin(const std::string& pin) {
    if (pin.size() != 4) return false;
    return std::all_of(pin.begin(), pin.end(), ::isdigit);
}

bool isValidName(const std::string& name) {
    if (name.empty()) return false;
    return std::all_of(name.begin(), name.end(),
        [](char c){ return std::isalpha(c) || c == ' '; });
}

bool isValidAmount(double amount) {
    return amount > 0.0;
}

// ─── Display helpers ──────────────────────────────────────────────────────────

void printHeader(const std::string& title) {
    printSeparator();
    int padding = static_cast<int>((50 - title.size()) / 2);
    std::cout << std::string(padding, ' ') << title << "\n";
    printSeparator();
}

void printSeparator() {
    std::cout << "  " << std::string(54, '-') << "\n";
}

void printSuccess(const std::string& msg) {
    std::cout << "\n  [OK]  " << msg << "\n\n";
}

void printError(const std::string& msg) {
    std::cout << "\n  [ERR] " << msg << "\n\n";
}

void printInfo(const std::string& msg) {
    std::cout << "\n  [i]   " << msg << "\n\n";
}

// ─── Hash (djb2) ─────────────────────────────────────────────────────────────

std::string hashPin(const std::string& pin) {
    unsigned long hash = 5381;
    for (char c : pin)
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    std::ostringstream oss;
    oss << std::hex << hash;
    return oss.str();
}

} // namespace Utils
