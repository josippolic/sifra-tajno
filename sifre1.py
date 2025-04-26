import os
import random
import time

FILE_NAME = "passwords-1.txt"
JOHN_LIST_PATH = os.path.expanduser("~/usr/share/wordlists/john.lst")
WIFILITE_LIST_PATH = os.path.expanduser("~/usr/share/wordlists/wifilite.txt")
CHARACTERS = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@Ł!&#$/<>*ł?£&{}[]€;:-_÷^"

def read_passwords():
    passwords = {}
    if os.path.exists(FILE_NAME):
        with open(FILE_NAME, "r") as file:
            for line in file:
                parts = line.strip().split()
                if len(parts) == 2:
                    passwords[parts[0]] = int(parts[1])
    return passwords

def write_passwords(passwords):
    with open(FILE_NAME, "w") as file:
        for password, count in passwords.items():
            file.write(f"{password} {count}\n")

def read_wordlist(path):
    blacklist = set()
    if os.path.exists(path):
        with open(path, "r", encoding="utf-8", errors="ignore") as file:
            for line in file:
                blacklist.add(line.strip())
    return blacklist

def generate_password(length):
    return ''.join(random.choice(CHARACTERS) for _ in range(length))

def password_generator():
    random.seed(time.time())
    passwords = read_passwords()
    blacklist = set()

    print("\n================== Password Generator App ==================\n")
    print("Select a wordlist to load:")
    print("1. john.lst")
    print("2. wifilite.txt")
    print("3. both (1 and 2)")
    choice = input("Enter your choice (1/2/3): ").strip()

    if choice == "1":
        blacklist = read_wordlist(JOHN_LIST_PATH)
    elif choice == "2":
        blacklist = read_wordlist(WIFILITE_LIST_PATH)
    elif choice == "3":
        blacklist = read_wordlist(JOHN_LIST_PATH).union(read_wordlist(WIFILITE_LIST_PATH))
    else:
        print("Invalid choice. Loading no wordlist.\n")

    print("\nSelect an option:")
    print("1. Generate a password manually")
    print("2. Generate a password automatically and avoid wordlist")
    print("3. Scan and generate new passwords (completely unique)")
    option = input("Enter your choice: ").strip()

    if option == "1":
        try:
            passlen = int(input("Enter the length of password (8-15): ").strip())
            if passlen < 8 or passlen > 15:
                print("\nPassword length should be between 8 and 15 characters.")
                return
        except ValueError:
            print("\nInvalid input. Password length should be a number.")
            return

        password = ""
        while True:
            password = generate_password(passlen)
            if password not in blacklist:
                break

        if password in passwords:
            passwords[password] += 1
            print(f"\n\nYour password is: {password}")
            print(f"This password has been generated {passwords[password]} times.")
        else:
            passwords[password] = 1
            print(f"\n\nYour password is: {password}")

    elif option == "2":
        try:
            passlen = int(input("Enter the length of password (8-15): ").strip())
            if passlen < 8 or passlen > 15:
                print("\nPassword length should be between 8 and 15 characters.")
                return
        except ValueError:
            print("\nInvalid input. Password length should be a number.")
            return

        password = ""
        while True:
            password = generate_password(passlen)
            if password not in blacklist:
                break

        if password in passwords:
            passwords[password] += 1
            print(f"\n\nYour password is: {password}")
            print(f"This password has been generated {passwords[password]} times.")
        else:
            passwords[password] = 1
            print(f"\n\nYour password is: {password}")

    elif option == "3":
        password = ""
        while True:
            passlen = random.randint(8, 15)
            password = generate_password(passlen)
            if password not in passwords and password not in blacklist:
                break

        passwords[password] = 1
        print(f"\n\nYour unique password is: {password}")

    else:
        print("Invalid option! Exiting program.")
        return

    write_passwords(passwords)

if __name__ == "__main__":
    password_generator()
