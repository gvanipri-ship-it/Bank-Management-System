cmake_minimum_required(VERSION 3.14)
project(BankManagement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(include)

set(SOURCES
    src/main.cpp
    src/Account.cpp
    src/Bank.cpp
    src/Utils.cpp
)

add_executable(bank_app ${SOURCES})

# Ensure the data directory exists at build time
add_custom_command(TARGET bank_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:bank_app>/data
    COMMENT "Creating data directory"
)
