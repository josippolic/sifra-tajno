#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>

using namespace std;

const string FILE_NAME = "passwords3.txt";
// Dodaj opcionalne wordlistove
const string WORDLIST_PATH_1 = "~/usr/share/wordlists/john.lst";
const string WORDLIST_PATH_2 = "~/usr/share/wordlists/wifilists/wifilite.txt";
const string CHARACTERS = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@Ł!&#$/<>*ł?£&{}[]€;:-_÷^";

// Učitaj prethodno generirane lozinke s brojačem
unordered_map<string, int> readPasswords() {
    unordered_map<string, int> passwords;
    ifstream file(FILE_NAME);
    if (file.is_open()) {
        string password;
        int count;
        while (file >> password >> count) {
            passwords[password] = count;
        }
        file.close();
    }
    return passwords;
}

// Zapiši sve lozinke i broj generiranja u datoteku
void writePasswords(const unordered_map<string, int>& passwords) {
    ofstream file(FILE_NAME);
    if (file.is_open()) {
        for (const auto& entry : passwords) {
            file << entry.first << " " << entry.second << "\n";
        }
        file.close();
    }
}

// Učitaj wordlist i zapamti sve lozinke koje se ne smiju koristiti
unordered_set<string> readWordlist(const string& path) {
    unordered_set<string> blacklist;
    ifstream file(path);
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            blacklist.insert(word);
        }
        file.close();
    }
    return blacklist;
}

// Generiraj slučajnu lozinku
string generatePassword(int length) {
    string password;
    for (int i = 0; i < length; i++) {
        password += CHARACTERS[rand() % CHARACTERS.length()];
    }
    return password;
}

// Glavna funkcija aplikacije
void passwordGenerator() {
    srand(time(0));
    unordered_map<string, int> passwords = readPasswords();

    // Upiši korisnika za odabir wordlist-a
    cout << "Select a wordlist to load:\n";
    cout << "1. john.lst\n";
    cout << "2. wifilite.txt\n";
    cout << "Enter 1 or 2: ";
    int choice;
    cin >> choice;

    string wordlistPath;
    if (choice == 1) {
        wordlistPath = WORDLIST_PATH_1;
    } else if (choice == 2) {
        wordlistPath = WORDLIST_PATH_2;
    } else {
        cout << "Invalid choice, defaulting to john.lst." << endl;
        wordlistPath = WORDLIST_PATH_1;
    }

    unordered_set<string> blacklist = readWordlist(wordlistPath);

    while (true) {
        cout << "\n================== Password Generator App ==================\n\n";
        cout << "Select an option:\n";
        cout << "1. Generate a password manually\n";
        cout << "2. Generate a password automatically and avoid wordlist\n";
        cout << "3. Scan and generate new passwords (completely unique)\n";
        cout << "Enter your choice: ";
        int option;
        cin >> option;

        if (option == 1) {
            int passlen;
            cout << "Enter the length of password (8-15): ";
            cin >> passlen;

            if (cin.fail() || passlen < 8 || passlen > 15) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nPassword length should be between 8 and 15 characters.\n";
                continue;
            }

            string password;
            // Generiraj dok ne dobijemo jedinstvenu lozinku (nije u wordlisti)
            do {
                password = generatePassword(passlen);
            } while (blacklist.find(password) != blacklist.end());

            if (passwords.find(password) != passwords.end()) {
                passwords[password]++;
                cout << "\n\nYour password is: " << password;
                cout << "\nThis password has been generated " << passwords[password] << " times.";
            } else {
                passwords[password] = 1;
                cout << "\n\nYour password is: " << password;
            }

            writePasswords(passwords);

            if (passlen == 15) {
                cout << "\nWarning: Password should not have 15 or more characters.\n";
            }
        }
        else if (option == 2) {
            int passlen;
            cout << "Enter the length of password (8-15): ";
            cin >> passlen;

            if (cin.fail() || passlen < 8 || passlen > 15) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nPassword length should be between 8 and 15 characters.\n";
                continue;
            }

            string password = generatePassword(passlen);

            if (passwords.find(password) != passwords.end()) {
                passwords[password]++;
                cout << "\n\nYour password is: " << password;
                cout << "\nThis password has been generated " << passwords[password] << " times.";
            } else {
                passwords[password] = 1;
                cout << "\n\nYour password is: " << password;
            }

            writePasswords(passwords);

        }
        else if (option == 3) {
            cout << "Scanning and generating completely new passwords (avoiding all wordlists and previously generated passwords)...\n";
            string password;
            // Generiraj lozinku koja nije u prethodnim lozinkama ni u wordlistima
            do {
                int passlen = rand() % (15 - 8 + 1) + 8; // Random length between 8 and 15
                password = generatePassword(passlen);
            } while (blacklist.find(password) != blacklist.end() || passwords.find(password) != passwords.end());

            passwords[password] = 1;
            cout << "\nGenerated new unique password: " << password;
            writePasswords(passwords);
        }

        string opt;
        cout << "\nDo you want to try again? (da/ne): ";
        cin >> opt;

        if (opt == "ne") {
            cout << "Exiting program....\n";
            break;
        } else if (opt != "da") {
            cout << "Please enter da/ne:\n";
        }
    }
}

int main() {
    passwordGenerator();
    return 0;
}
