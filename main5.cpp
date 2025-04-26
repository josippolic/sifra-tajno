#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const string FILE_NAME = "passwords5.txt";
const string JOHN_LIST_PATH = "~/usr/share/wordlists/john.lst";
const string WIFILITE_LIST_PATH = "~/usr/share/wordlists/wifilite.txt";
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
    unordered_set<string> blacklist;

    // Dodajemo opciju za odabir wordliste
    int choice;
    cout << "\n================== Password Generator App ==================\n\n";
    cout << "Select a wordlist to load:\n";
    cout << "1. john.lst\n";
    cout << "2. wifilite.txt\n";
    cout << "3. both (1 and 2)\n";
    cout << "Enter your choice (1/2/3): ";
    cin >> choice;

    // Učitavamo wordlist prema odabiru
    if (choice == 1) {
        blacklist = readWordlist(JOHN_LIST_PATH);
    } else if (choice == 2) {
        blacklist = readWordlist(WIFILITE_LIST_PATH);
    } else if (choice == 3) {
        unordered_set<string> johnBlacklist = readWordlist(JOHN_LIST_PATH);
        unordered_set<string> wifiliteBlacklist = readWordlist(WIFILITE_LIST_PATH);
        blacklist.insert(johnBlacklist.begin(), johnBlacklist.end());
        blacklist.insert(wifiliteBlacklist.begin(), wifiliteBlacklist.end());
    } else {
        cout << "Invalid choice. Loading no wordlist.\n";
    }

    // Odabir opcije za generiranje lozinke
    int option;
    cout << "\nSelect an option:\n";
    cout << "1. Generate a password manually\n";
    cout << "2. Generate a password automatically and avoid wordlist\n";
    cout << "3. Scan and generate new passwords (completely unique)\n";
    cout << "Enter your choice: ";
    cin >> option;

    switch (option) {
        case 1: {
            // Ručno generiranje lozinke
            int passlen;
            cout << "Enter the length of password (8-15): ";
            cin >> passlen;

            if (cin.fail() || passlen < 8 || passlen > 15) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nPassword length should be between 8 and 15 characters.\n";
                break;
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
            break;
        }

        case 2: {
            // Automatsko generiranje lozinke i izbjegavanje wordlist
            int passlen;
            cout << "Enter the length of password (8-15): ";
            cin >> passlen;

            if (cin.fail() || passlen < 8 || passlen > 15) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\nPassword length should be between 8 and 15 characters.\n";
                break;
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
            break;
        }

        case 3: {
            // Skeniranje i generiranje potpuno novih lozinki
            string password;
            bool unique = false;
            while (!unique) {
                password = generatePassword(rand() % 8 + 8);  // Generira lozinku duljine od 8 do 15
                if (passwords.find(password) == passwords.end() && blacklist.find(password) == blacklist.end()) {
                    unique = true;
                }
            }

            passwords[password] = 1;
            cout << "\n\nYour unique password is: " << password;
            break;
        }

        default:
            cout << "Invalid option! Exiting program.\n";
            return;
    }

    // Spremanje svih lozinki u datoteku
    writePasswords(passwords);
}

int main() {
    passwordGenerator();
    return 0;
}
