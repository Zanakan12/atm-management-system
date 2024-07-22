#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 200

// Function to read an account from a file
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
    return fscanf(ptr, "\n%d %d %s %d %d/%d/%d %s %d %lf %s",
                &r->id,
		        &r->userId,
		        name,
                &r->accountNbr,
                &r->deposit.month,
                &r->deposit.day,
                &r->deposit.year,
                r->country,
                &r->phone,
                &r->amount,
                r->accountType) != EOF;
}

// Function to save an account to a file
void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
    fprintf(ptr, "\n\n%d %d %s %d %d/%d/%d %s %d %.2lf %s",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

// Function to count the number of lines in a file
int countLinesInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    int lines = 0;
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

// Function to print centered text
void printCentered(const char *text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int width = w.ws_col;
    int textLen = strlen(text);
    int padding = (width - textLen) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Function to prompt the user to stay or return
void stayOrReturn(int notGood, void f(struct User u), struct User u) {
    int option;
    if (notGood == 0) {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1) {
        system("clear");
        mainMenu(u);
    } else {
        system("clear");
        exit(1);
    }
}

// Function to indicate success
void success(struct User u) {
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1) {
        mainMenu(u);
    } else if (option == 0) {
        exit(1);
    } else {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

// Function to create a new account
void createNewAcc(struct User u) {
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    bool accountExists = false;
    fseek(pf, 0, SEEK_SET);  // Reset file pointer to start for reading
    while (getAccountFromFile(pf, userName, &cr)) {
        if (strcmp(userName, u.name) == 0) {
            u.id = cr.userId;  // Set u.id to the userId of the first found account
            if (cr.accountNbr == r.accountNbr) {
                printf("✖ This Account already exists for this user\n\n");
                goto noAccount;
            }
            accountExists = true;
        }
    }
    if (!accountExists) {
        int lineCount = countLinesInFile(RECORDS);
        u.id = (lineCount == -1) ? 0 : lineCount;  // Set new user ID
    }

    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    fseek(pf, 0, SEEK_END);  // Move file pointer to the end for appending
    int lineCount = countLinesInFile(RECORDS)-1;
    r.id = (lineCount == -1) ? 0 : lineCount;  // Set r.id based on line count

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

// Function to check all accounts
void checkAllAccounts(struct User u) {
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0) {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \nCountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                r.accountNbr,
                r.deposit.day,
                r.deposit.month,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

// Function to update account information
void updateaccountinformation(struct User u) {
    int id;
    int choice;
    char newPhoneNumber[15];
    char newCountry[50];
    char line[MAX_LINE_LENGTH];
    int accountFound = 0;

    printf("What is the account number you want to change: ");
    scanf("%d", &id);

    printf("\nWhich information do you want to update:\n1 -> phone number\n2 -> country\n");
    scanf("%d", &choice);

    FILE *file = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error opening file");
        return;
    }
    bool myBoolean = true;
    while (fgets(line, sizeof(line), file)) {
        int fileId;
        char name[50], date[20], country[50], phoneNumber[15], otherData[100];
        double balance;
        int otherId1, otherId2;
        
        sscanf(line, "%d %d %s %d %s %s %s %lf %s", &fileId, &otherId1, name, &otherId2, date, country, phoneNumber, &balance, otherData);

        if (otherId2== id) {
            accountFound = 1;
            if (choice == 1 && myBoolean) {
                printf("Enter the new phone number: ");
                scanf("%s", newPhoneNumber);
                fprintf(tempFile, "%d %d %s %d %s %s %s %.2f %s\n", fileId, otherId1, name, otherId2, date, country, newPhoneNumber, balance, otherData);
                printf("Phone number updated to: %s\n", newPhoneNumber);
                myBoolean = false;
            } else if (choice == 2 && myBoolean) {
                printf("Enter the new country: ");
                scanf("%s", newCountry);
                fprintf(tempFile, "%d %d %s %d %s %s %s %.2f %s\n\n", fileId, otherId1, name, otherId2, date, newCountry, phoneNumber, balance, otherData);
                printf("Country updated to: %s\n", newCountry);
                myBoolean = false;
            } 
        } else {
            fprintf(tempFile, "%s", line);
        }
        
    }

    if (!accountFound) {
        printf("Account with ID %d not found.\n", id);
    }

    fclose(file);
    fclose(tempFile);

    remove(RECORDS);
    rename("temp.txt", RECORDS);
    success(u);
}

// Function to handle user registration
void Registration() {
    system("clear");
    struct User u;
    char name[50] = {0}; // Array to store name
    char pwd[50] = {0};  // Array to store password

    FILE *file = fopen(USERS, "a");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    int line = countLinesInFile(USERS);
    if (line == -1) {
        perror("Failed to read the file");
        fclose(file);
        return;
    }

    printCentered("Bank Management System\n");
    printCentered("Enter your desired username: ");
    scanf("%49s", name); // Use %49s to prevent buffer overflow

    if (lookforusedname(USERS, name)) {
        system("clear");
        printCentered("Name used, please try with another name!!!");
        char exit = '0';
        while (exit != '1') { 
            printf("[1]- Register\n[ctrl+c]- Exit\n");
            scanf(" %c", &exit); 
        }
        Registration();
        return;
    }

    printCentered("Enter your desired password: ");
    scanf("%49s", pwd);

    strcpy(u.name, name);
    strcpy(u.password, pwd);
    u.id = line;

    fprintf(file, "%d %s %s\n", u.id, u.name, u.password);

    fclose(file);
    printCentered("User registered successfully.\n");
    loginMenu(name,pwd);
}

// Function to check if a username is already used
bool lookforusedname(const char *filename, const char *element) {
    char line[1024];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return false;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, element) != NULL) {
            fclose(file);
            return true;  // Return true if the element is found
        }
    }

    fclose(file);
    return false;  // Return false if the element is not found after reading the entire file
}

 // Function to check an account based on account ID
void checkanaccounts(struct User u) {
    int accountNbr;
    struct Record r;
    char userName[50];

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

    printf("Enter the account ID to check: ");
    scanf("%d", &accountNbr);

    bool accountFound = false;

    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountNbr) {
            printf("\nAccount number: %d\nUser ID: %d\nUser Name: %s\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2lf\nType Of Account: %s\n",
                r.accountNbr,
                r.userId,
                userName,
                r.deposit.day,
                r.deposit.month,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
            accountFound = true;
            break;
        }
    }

    if (strcmp(r.accountType, "saving") == 0) {
        // Calculer le résultat
        double result = (r.amount * 0.07) / 12;

        // Afficher le résultat avec printf
        printf("\n\n-> You will get $ %0.2f as interest on day 10 of every month.\n", result);
    }else if (strcmp(r.accountType, "fixed01") == 0) {

        // Calculer le résultat
        double result = (r.amount * 0.04) / 12;

        // Afficher le résultat avec printf
        printf("\n\n-> You will get $ %.2f as interest on %d/%d/%d.\n", result, r.deposit.month, r.deposit.day, r.deposit.year + 1);

    }else if (strcmp(r.accountType, "fixed02") == 0) {

        // Calculer le résultat
        double result = (r.amount * 0.05);

        // Afficher le résultat avec printf
        printf("\n\n-> You will get $ %.2f as interest on %d/%d/%d.\n", result, r.deposit.month, r.deposit.day, r.deposit.year + 2);
    }else if (strcmp(r.accountType, "fixed03") == 0) {

        // Calculer le résultat
        double result = (r.amount * 0.08);

        // Afficher le résultat avec printf
        printf("\n\n-> You will get $ %.2f as interest on %d/%d/%d.\n", result*3, r.deposit.month, r.deposit.day, r.deposit.year + 3);
    }else{
        printf("\n\n->You will not get interests because the account is of type current\n");
    }


    if (!accountFound) {
        printf("✖ Account with ID %d not found.\n", accountNbr);
    }

    fclose(pf);
    success(u);
}

void makeTransaction(struct User u) {
    int accountNbr;
    struct Record r;
    int choice;
    int amount;
    char userName[50]; // Added userName to pass to the function

    FILE *pf = fopen(RECORDS, "r"); // Open original file in read mode
    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

    printf("Enter the account ID to make transaction: ");
    scanf("%d", &accountNbr);

    FILE *tempPf = fopen("temp.txt", "w"); // Open temporary file in write mode
    if (tempPf == NULL) {
        perror("Temporary file opening failed");
        fclose(pf);
        return;
        success(u);
    }

    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountNbr) {
            printf("Do you want to:\n     1 -> Withdraw\n     2 -> Deposit\n\nEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter the amount you want to withdraw: $");
                    scanf("%d", &amount);
                    if (amount > r.amount) {
                        printf("✖ Insufficient funds.\n");
                    } else {
                        r.amount -= amount;
                        printf("✓ Withdrawal successful. New balance: %.2f\n", r.amount);
                        
                        // Update withdrawal date
                        printf("Enter the withdrawal date (MM DD YYYY): ");
                        scanf("%d %d %d", &r.withdraw.month, &r.withdraw.day, &r.withdraw.year);
                    }
                    break;
                
                case 2:
                    printf("Enter the amount you want to deposit: $");
                    scanf("%d", &amount);
                    r.amount += amount;
                    printf("✓ Deposit successful. New balance: %.2f\n", r.amount);
                    
                    // Update deposit date
                    printf("Enter the deposit date (MM DD YYYY): ");
                    scanf("%d %d %d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
                    break;

                default:
                    printf("✖ Invalid choice.\n");
                    break;
            }
        }

        // Write the record to the temporary file
        fprintf(tempPf, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n\n", 
                r.id, r.userId, userName, r.accountNbr, 
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempPf);

    // Replace original file with updated temporary file
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    success(u);
}

void deleteAccount(struct User u) {
    int accountNbr;
    struct Record r;
    char userName[50];

    FILE *pf = fopen(RECORDS, "r"); // Open original file in read mode
    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

    FILE *tempPf = fopen("temp.txt", "w"); // Open temporary file in write mode
    if (tempPf == NULL) {
        perror("Temporary file opening failed");
        fclose(pf);
        return;
    }

    printf("Enter the account ID to delete: ");
    scanf("%d", &accountNbr);

    bool accountFound = false;

    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountNbr) {
            accountFound = true;
            printf("Account with ID %d has been deleted.\n", accountNbr);
            continue; // Skip writing this record to the temporary file
        }
        // Write the remaining records to the temporary file
        fprintf(tempPf, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n\n", 
                r.id, r.userId, userName, r.accountNbr, 
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    if (!accountFound) {
        printf("✖ Account with ID %d not found.\n", accountNbr);
    }

    fclose(pf);
    fclose(tempPf);

    // Replace original file with updated temporary file
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    success(u);
}

void transferOwner(struct User u) {
    int accountNbr;
    struct Record r;
    char userName[50];
    int newUserId;
    char newUserName[100];
    FILE *pf = fopen(RECORDS, "r"); // Open original file in read mode
    if (pf == NULL) {
        perror("File opening failed");
        return;
    }

    FILE *tempPf = fopen("temp.txt", "w"); // Open temporary file in write mode
    if (tempPf == NULL) {
        perror("Temporary file opening failed");
        fclose(pf);
        return;
    }

    printf("Enter the account ID to transfer ownership: ");
    scanf("%d", &accountNbr);
    printf("Enter the new owner's user ID: ");
    scanf("%d", &newUserId);
    printf("Enter the new owner's name: ");
    scanf("%s", newUserName);

    bool accountFound = false;

    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountNbr) {
            accountFound = true;
            r.userId = newUserId;
            strncpy(r.name, newUserName, sizeof(r.name));
            printf("Ownership of account ID %d has been transferred to %s (User ID: %d).\n", accountNbr, newUserName, newUserId);
        }

        // Write the record to the temporary file
        fprintf(tempPf, "\n%d %d %s %d %d/%d/%d %s %d %.2f %s\n", 
                r.id, r.userId, r.name, r.accountNbr, 
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    if (!accountFound) {
        printf("✖ Account with ID %d not found.\n", accountNbr);
    }

    fclose(pf);
    fclose(tempPf);

    // Replace original file with updated temporary file
    remove(RECORDS);
    rename("temp.txt", RECORDS);
    success(u);
}
