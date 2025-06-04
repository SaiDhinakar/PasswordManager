# Secure Password Manager

<div align="center">

![Password Manager Logo](https://img.shields.io/badge/Security-Password%20Manager-blue)        ![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20MacOS-lightgrey)

![Version](https://img.shields.io/badge/version-1.0.0-orange)

</div>

A secure, lightweight command-line password manager that stores all passwords locally with encryption. This application helps you manage passwords for different applications while ensuring they remain secure and easily accessible.

## 🔐 Security Features

- **Local Storage**: All passwords are stored locally on your device
- **Strong Encryption**: Data is encrypted with XOR cipher and key stretching
- **No Internet Connection**: No data is sent over the internet
- **Brute Force Protection**: After 3 failed login attempts, access is blocked
- **Master Password**: Single master password protects all your stored passwords

## 📋 Features

- **Store Passwords**: Securely save passwords for different applications
- **Organize By App**: Group passwords by application name
- **View Passwords**: Display passwords in an organized tree structure
- **Update Passwords**: Easily update existing passwords
- **Remove Passwords**: Delete passwords you no longer need
- **Command-Line Interface**: Simple and efficient terminal-based user interface
- **Cross-Platform**: Works on Windows, Linux, and MacOS

## 📸 Screenshots

### Login Screen

```
Enter Username: admin
Enter password for admin: ********
Login Successful
```

### Welcome Screen

```
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║                SECURE PASSWORD MANAGER                     ║
║                                                            ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  • All passwords are stored locally on your device         ║
║  • Data is encrypted                                       ║
║  • No data is sent over the internet                       ║
║  • After 3 failed login attempts, access is blocked        ║
║                                                            ║
║  Welcome, admin                                            ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝

Type 'help' for available commands
```

### Help Screen

```
╔═══════════════════════════════════════════════════════════╗
║                   AVAILABLE COMMANDS                      ║
╠═══════════════════════════════════════════════════════════╣
║  add password <app> <user> <pass>                         ║
║     → Add a new password to the secure vault              ║
║                                                           ║
║  update password <app> <user> <new_pass>                  ║
║     → Update an existing password                         ║
║                                                           ║
║  remove password <app> <user>                             ║
║     → Remove a password from the vault                    ║
║                                                           ║
║  list                                                     ║
║     → Display all passwords in a tree structure           ║
║                                                           ║
║  show <app>                                               ║
║     → Show passwords for a specific application           ║
║                                                           ║
║  help                                                     ║
║     → Display this help message                           ║
║                                                           ║
║  exit                                                     ║
║     → Exit the program and secure all data                ║
╚═══════════════════════════════════════════════════════════╝

🔐 SECURITY REMINDER:
   Your passwords are securely encrypted and stored locally.
   Never share your master password.
```

### Password List View

```
Password Manager Tree:
======================

├── Facebook
│   ├── john_doe : MySecureFBPass123
│   └── jane_doe : AnotherSafePass!
└── Gmail
    └── john.doe : GmailPass#2023

Press RETURN / ENTER to continue…
```

## 🚀 Installation

### Prerequisites

- C++ compiler (GCC or compatible)
- Make build system

### Building from Source

1. Clone this repository:

```bash
git clone https://github.com/SaiDhinakar/PasswordManager.git
cd PasswordManager
```

2. Compile the project:

```bash
make
```

3. Run the password manager:

```bash
./PasswordManager
```

## 📖 Usage

### First Time Setup

On first run, you'll be prompted to create an admin account:

```
First time setup. Creating admin account.
Enter admin username: admin
Enter admin password: ********
Admin account created successfully!
```

### Basic Commands

- Adding a password: `add password app_name username password`
- Updating a password: `update password app_name username new_password`
- Removing a password: `remove password app_name username`
- Listing all passwords: `list`
- Showing passwords for a specific app: `show app_name`
- Getting help: `help`
- Exiting the program: `exit`

## 🔧 Technical Details

### Security Implementation

- Secure encryption to protect your stored passwords
- Enhanced security using password salting techniques
- Multiple security layers to protect against brute force attacks
- Binary data encoding for reliable storage

### Storage Format

- Passwords are stored in an encrypted binary file (`password_data.bin`)
- Structured data format with clear separation between user credentials and app data

### Data Structure

- Applications are indexed and contain name and associated usernames/passwords
- User credentials are stored in a map for fast retrieval

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 👨‍💻 Author

- **Sai Dhinakar S** - [GitHub](https://github.com/SaiDhinakar)

---

⭐ If you find this project useful, please give it a star on GitHub! ⭐
