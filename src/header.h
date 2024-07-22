#ifndef HEADER_H
#define HEADER_H
#include <stdbool.h>
#include <stdio.h>

extern const char *USERS;
extern const char *RECORDS;

struct Date {
    int month, day, year;
};

// All fields for each record of an account
struct Record {
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User {
    int id;
    char name[50];
    char password[50];
};

// Authentication functions
void loginMenu(char name[50], char pass[50]);
void registerMenu(char name[50], char pass[50]);
const char *getPassword(struct User u);

// System functions
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void updateaccountinformation(struct User u);
void Registration();
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);
void stayOrReturn(int notGood, void f(struct User u), struct User u);
void success(struct User u);
int countLinesInFile(const char *filename);
bool lookforusedname(const char *filename, const char *element);
void checkanaccounts(struct User u);
void makeTransaction(struct User u);
void writeAccountToFile(FILE *pf, struct Record *r);
void deleteAccount(struct User u);
void transferOwner(struct User u);
#endif
