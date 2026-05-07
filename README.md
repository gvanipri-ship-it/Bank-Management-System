# Bank Management System — C++

A console-based banking application built with C++17, demonstrating OOP design and file-based persistence.

## Features

| Feature | Description |
|---|---|
| Create Account | Open a Savings or Checking account with a 4-digit PIN |
| Deposit | Add funds to any account |
| Withdraw | Withdraw funds with PIN authentication |
| Transfer | Move funds between two accounts |
| Balance Check | View balance with PIN verification |
| View Details | Display full account information |
| List All | Tabular view of every account |
| Search | Find accounts by holder name |
| Delete Account | Remove an account (PIN required) |
| Persistent Storage | All data saved to `data/accounts.dat` |

## Project Structure

```
bank_management/
├── include/
│   ├── Account.h       # Account entity
│   ├── Bank.h          # Bank operations & file I/O
│   └── Utils.h         # Input helpers, validation, display
├── src/
│   ├── Account.cpp
│   ├── Bank.cpp
│   ├── Utils.cpp
│   └── main.cpp        # Menu-driven UI
├── data/               # Created at runtime
│   └── accounts.dat    # Pipe-delimited account records
├── CMakeLists.txt
└── Makefile
```

## Build & Run

### Using Make (Linux / macOS / Git Bash on Windows)
```bash
cd bank_management
make
./bank_app
```

### Using CMake
```bash
cd bank_management
cmake -B build
cmake --build build
./build/bank_app        # Linux/macOS
build\Debug\bank_app    # Windows
```

### Using g++ directly
```bash
cd bank_management
g++ -std=c++17 -Iinclude src/*.cpp -o bank_app
./bank_app
```

## Data Format

Accounts are stored in `data/accounts.dat`, one per line:

```
1001|Alice Smith|<hashed_pin>|SAVINGS|1500.00
1002|Bob Jones|<hashed_pin>|CHECKING|250.75
```

PINs are hashed with the djb2 algorithm before storage — they are never saved in plain text.

## OOP Design

- **`Account`** — encapsulates account state and operations (`deposit`, `withdraw`, `verifyPin`, serialization)
- **`Bank`** — manages a collection of accounts, handles file I/O, and exposes high-level banking operations
- **`Utils`** — stateless utility namespace for input handling, validation, and display formatting
