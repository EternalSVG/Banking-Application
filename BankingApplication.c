/*************************

By: Sohan Yarlagadda
CS 36, Summer 2024, Glenn Cuevas


Page
1
of 3
Scope: Files
Banking application.
Create a Start menu with the following options.
- Open Account
- Login
- Exit
Create a Main menu with the following options:
- Check Balance
- Deposit
- Withdraw
- Show transactions
- Exit
Create structure(s) to support the following information:
- Owner name
- Password
- Account balance
- Transaction Count
- Transaction amount
- Starting Balance
- Ending Balance
Behavior:
1. Open Account
a. Get the user name & password, how much they are opening the account with.
Transaction Count starts at 0.
b. Launch Main menu, use this new account as context.
2. Login
a. Ask for account name and password.
b. Load the account details from the file with that account name.
i. If the file does not exist, or the password does not match, tell them invalid
username/password.
c. Launch the Main menu, use this account as the context.
3. Check Balance
a. Show current balance.
4. Deposit
a. Ask the user how much to deposit (positive amount only).
b. Add the amount to the balance and show the new balance.
c. Create a transaction record.
5. Withdraw
a. Same as deposit, but reduce the balance.
6. Show Transactions
a. Show all the transactions for this account.
Transaction Behavior.
When making a transaction, that record is saved to the transactions file for that account.
Files:
There are two types of files.
- Account Info
- Holds onto information, such as balance and password
- Transaction Details
- Holds the transactions for a specific account.
File Structures (Recommended)
1. Account Name.info
a. Password
b. Balance
2. Account Name.tran
a. Starting Balance
b. Transaction Amount
c. Ending Balance
Example Files
File name:
- Bob.info
File contents:
- P@s$W0rd
- 155.25
File name:
- Bob.tran
File contents:
- 0 100 100
- 100 20 120
- 120 40 160
- 160 -4.75 155.25
First value is the starting amount, 2nd is the transaction amount, third is the balance after. The
first record is the amount the account is opened with, so we started at 0, and opened the
account with $100. The 2nd line we had $100, deposited $20, ending with $120.

***************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_TRANSACTIONS 100

typedef struct {
    float startingBalance;
    float transactionAmount;
    float endingBalance;
} Transaction;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    float balance;
    int transactionCount;
    Transaction transactions[MAX_TRANSACTIONS];
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void openAccount();
void login();
void checkBalance(Account *account);
void deposit(Account *account);
void withdraw(Account *account);
void showTransactions(Account *account);
void exitProgram();
void mainMenu(Account *account);
void saveAccountInfo(Account *account);
int loadAccountInfo(const char *name, Account *account);
void saveTransaction(const char *name, float startingBalance, float transactionAmount, float endingBalance);
void loadTransactions(const char *name, Account *account);

int main() {
    int choice;

    do {
        printf("1. Open Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter your choice (numerically): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                openAccount();
                break;
            case 2:
                login();
                break;
            case 3:
                exitProgram();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 3);
    return 0;
}

void openAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }
    Account newAccount;

    printf("\nEnter your username: ");
    scanf("%s", newAccount.name);

    printf("\nEnter your password: ");
    scanf("%s", newAccount.password);

    printf("\nEnter the amount to open the account with: ");
    scanf("%f", &newAccount.balance);

    newAccount.transactionCount = 0;
    accounts[accountCount] = newAccount;
    accountCount++;

    saveAccountInfo(&newAccount);
    saveTransaction(newAccount.name, 0.0, newAccount.balance, newAccount.balance);

    printf("\nAccount opened successfully!\n");
    mainMenu(&accounts[accountCount - 1]);
}

void login() {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("\nEnter your username: ");
    scanf("%s", name);

    printf("\nEnter your password: ");
    scanf("%s", password);

    for (int i = 0; i < accountCount; i++) {
        if (strcmp(name, accounts[i].name) == 0 && strcmp(password, accounts[i].password) == 0) {
            printf("\nLogin successful!\n");
            loadTransactions(name, &accounts[i]);
            mainMenu(&accounts[i]);
            return;
        }
    }
    printf("\nInvalid username or password.\n");
}

void checkBalance(Account *account) {
    printf("\nYour balance is: $%.2f\n", account->balance);
}

void deposit(Account *account) {
    float amount;

    printf("\nEnter the amount to deposit: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("\nInvalid amount. Please enter a positive value.\n");
    } else {
        float oldBalance = account->balance;
        account->balance += amount;
        account->transactions[account->transactionCount] = (Transaction){oldBalance, amount, account->balance};
        account->transactionCount++;
        saveTransaction(account->name, oldBalance, amount, account->balance);
        printf("\nDeposit successful!\n");
    }
}

void withdraw(Account *account) {
    float amount;

    printf("\nEnter the amount to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("\nInvalid amount. Please enter a positive value.\n");
    } else if (amount > account->balance) {
        printf("\nInsufficient balance.\n");
    } else {
        float oldBalance = account->balance;
        account->balance -= amount;
        account->transactions[account->transactionCount] = (Transaction){oldBalance, -amount, account->balance};
        account->transactionCount++;
        saveTransaction(account->name, oldBalance, -amount, account->balance);
        printf("\nWithdrawal successful!\n");
    }
}

void showTransactions(Account *account) {
    printf("\nTransaction History:\n");
    for (int i = 0; i < account->transactionCount; i++) {
        Transaction t = account->transactions[i];
        printf("Starting Balance: $%.2f, Transaction Amount: $%.2f, Ending Balance: $%.2f\n", t.startingBalance, t.transactionAmount, t.endingBalance);
    }
}

void exitProgram() {
    printf("\nExiting the program. Goodbye!\n");
    exit(0);
}

void mainMenu(Account *account) {
    int choice;
    do {
        printf("\nMain Menu:\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Show Transactions\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBalance(account);
                break;
            case 2:
                deposit(account);
                break;
            case 3:
                withdraw(account);
                break;
            case 4:
                showTransactions(account);
                break;
            case 5:
                exitProgram();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 5);
}

void saveAccountInfo(Account *account) {
    char filename[MAX_NAME_LENGTH + 5];
    sprintf(filename, "%s.info", account->name);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(file, "%s\n", account->password);
    fprintf(file, "%.2f\n", account->balance);
    fclose(file);
}

int loadAccountInfo(const char *name, Account *account) {
    char filename[MAX_NAME_LENGTH + 5];
    sprintf(filename, "%s.info", name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }
    fscanf(file, "%s", account->password);
    fscanf(file, "%f", &account->balance);
    fclose(file);
    return 1;
}

void saveTransaction(const char *name, float startingBalance, float transactionAmount, float endingBalance) {
    char filename[MAX_NAME_LENGTH + 5];
    sprintf(filename, "%s.tran", name);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(file, "%.2f %.2f %.2f\n", startingBalance, transactionAmount, endingBalance);
    fclose(file);
}

void loadTransactions(const char *name, Account *account) {
    char filename[MAX_NAME_LENGTH + 5];
    sprintf(filename, "%s.tran", name);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    float startingBalance, transactionAmount, endingBalance;

    while (fscanf(file, "%f %f %f", &startingBalance, &transactionAmount, &endingBalance) == 3) {
        account->transactions[account->transactionCount] = (Transaction){startingBalance, transactionAmount, endingBalance};
        account->transactionCount++;
    }
    fclose(file);
}