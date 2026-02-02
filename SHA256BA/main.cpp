#include <iostream>
#include <string>
#include <cctype>
#include <limits>

#define NOMINMAX            //  stops Windows from breaking std::max / std::min
#include <windows.h>        //  for console color

#include "SHA256.h"

// Safe integer input (prevents infinite WRONG loop)
int getIntChoice() {
    int choice;

    while (!(std::cin >> choice)) {
        std::cout << "Please enter a number.\nChoice: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

// Console color helpers (Windows)
void setRedText() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void resetTextColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Handles admin-only menu access
void adminAction(const std::string& role) {
    if (role != "admin") {
        std::cout << "WOAH THERE BUDDY! ADMIN ONLY!\n";
    }
    else {
        std::cout << "Welcome to the ADMIN menu.\n";
    }
}

// Handles user menu access for users and admins
void userAction(const std::string& role) {
    if (role == "user" || role == "admin") {
        std::cout << "Welcome to the USER menu.\n";
    }
}

// Number of positions each letter is shifted in the cipher
const int SHIFT = 3;

// Encrypts a message by shifting letters forward
std::string encryptMessage(std::string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        char c = text[i];

        if (isalpha(static_cast<unsigned char>(c))) {
            char base = islower(static_cast<unsigned char>(c)) ? 'a' : 'A';
            c = (c - base + SHIFT) % 26 + base;
        }

        text[i] = c;
    }

    return text;
}

// Decrypts a message by shifting letters backward
std::string decryptMessage(std::string text) {
    for (int i = 0; i < (int)text.length(); i++) {
        char c = text[i];

        if (isalpha(static_cast<unsigned char>(c))) {
            char base = islower(static_cast<unsigned char>(c)) ? 'a' : 'A';
            c = (c - base - SHIFT + 26) % 26 + base;
        }

        text[i] = c;
    }

    return text;
}

// SHA-256 hashing menu
void sha256Menu() {
    while (true) {
        std::cout << "\n--- SHA-256 Hash Menu ---\n";
        std::cout << "1) Hash a string\n";
        std::cout << "2) Back\n";
        std::cout << "Choice: ";

        int option = getIntChoice();

        if (option == 1) {
            std::string input;
            std::cout << "Enter text to hash: ";
            std::getline(std::cin, input);

            SHA256 sha;
            sha.update(input);
            auto digest = sha.digest();
            std::string hashString = SHA256::toString(digest);

            std::cout << "\n----- RESULTS -----\n";
            std::cout << "Input Text: " << input << "\n";
            std::cout << "SHA-256 Hash: ";
            setRedText();
            std::cout << hashString << "\n";
            resetTextColor();
        }
        else if (option == 2) {
            break;
        }
        else {
            std::cout << "WRONG.\n";
        }
    }
}

// Crypto Tools Menu (Caesar + SHA-256)
void cryptoMenu() {
    while (true) {
        std::cout << "\n--- Crypto Tools Menu ---\n";
        std::cout << "1) Encrypt Message (Caesar)\n";
        std::cout << "2) Decrypt Message (Caesar)\n";
        std::cout << "3) SHA-256 Hash Menu\n";
        std::cout << "4) Back\n";
        std::cout << "Choice: ";

        int option = getIntChoice();

        if (option == 1) {
            std::string message;
            std::cout << "Enter message to encrypt: ";
            std::getline(std::cin, message);

            std::string encrypted = encryptMessage(message);

            std::cout << "Encrypted Message: ";
            setRedText();
            std::cout << encrypted << "\n";
            resetTextColor();
        }
        else if (option == 2) {
            std::string message;
            std::cout << "Enter message to decrypt: ";
            std::getline(std::cin, message);

            std::string decrypted = decryptMessage(message);
            std::cout << "Decrypted Message: " << decrypted << "\n";
        }
        else if (option == 3) {
            sha256Menu();
        }
        else if (option == 4) {
            break;
        }
        else {
            std::cout << "WRONG.\n";
        }
    }
}

// Program entry point and login/menu controller
int main() {
    std::string username;
    std::string password;
    std::string role;

    std::cout << "CIA Log In Screen\n";

    while (true) {
        std::cout << "Enter username (or type exit): ";
        std::getline(std::cin, username);

        if (username == "exit") {
            std::cout << "Later!\n";
            break;
        }

        std::cout << "Enter password: ";
        std::getline(std::cin, password);

        if (username == "Chubbs" && password == "12345") {
            role = "admin";
        }
        else if (username == "Cook" && password == "12354") {
            role = "user";
        }
        else if (username == "Norm" && password == "12354") {
            role = "user";
        }
        else {
            std::cout << "HACKER DETECTED! RELEASE THE HOUNDS!\n\n";
            continue;
        }

        std::cout << "Welcome Agent. Your role is: " << role << "\n";

        bool loggedIn = true;
        while (loggedIn) {
            std::cout << "\nChoose an option:\n";
            std::cout << "1) Admin Menu\n";
            std::cout << "2) User Menu\n";
            std::cout << "3) Encrypt / Decrypt / Hash Tools\n";
            std::cout << "4) Logout\n";
            std::cout << "Enter choice: ";

            int choice = getIntChoice();

            if (choice == 1) {
                adminAction(role);
            }
            else if (choice == 2) {
                userAction(role);
            }
            else if (choice == 3) {
                if (role == "user" || role == "admin") {
                    cryptoMenu();
                }
            }
            else if (choice == 4) {
                std::cout << "Have a good day Sweetie!\n\n";
                loggedIn = false;
            }
            else {
                std::cout << "WRONG.\n";
            }
        }
    }

    return 0;
}
