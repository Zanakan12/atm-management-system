#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mainMenu(struct User u) {
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option) {
        case 1:
            createNewAcc(u);
            break;
        case 2:
            updateaccountinformation(u);
            break;
        case 3:
            checkanaccounts(u);
            break;
        case 4:
            checkAllAccounts(u);
            break;
        case 5:
            makeTransaction(u);
            break;
        case 6:
            deleteAccount(u);
            break;
        case 7:
            transferOwner(u);
            break;
        case 8:
            exit(1);
            break;
        default:
            printf("Invalid operation!\n");
    }
}

void initMenu(struct User *u) {
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r) {
        scanf("%d", &option);
        switch (option) {
            case 1:
                loginMenu(u->name, u->password);
                const char* stored_password = getPassword(*u);
                if (stored_password && strcmp(u->password, stored_password) == 0) {
                    printf("\n\nPassword Match!");
                } else {
                    printf("\nWrong password!! or User Name\n");
                    exit(1);
                }
                r = 1;
                break;
            case 2:
                Registration();
                r = 1;
                break;
            case 3:
                exit(1);
                break;
            default:
                printf("Insert a valid operation!\n");
                r=1;
                break;
        }
    }
}

int main() {
    struct User u;
    initMenu(&u);
    mainMenu(u);
    return 0;
}
