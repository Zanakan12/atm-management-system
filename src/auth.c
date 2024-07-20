#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *USERS = "./data/users.txt";
const char *RECORDS = "./data/records.txt";

void loginMenu(char name[50], char password[50]) {
    printf("Enter your username: ");
    scanf("%49s", name);
    printf("Enter your password: ");
    scanf("%49s", password);
}

void registerMenu(char name[50], char password[50]) {
    printf("Enter your username: ");
    scanf("%49s", name);
    printf("Enter your password: ");
    scanf("%49s", password);
}

const char* getPassword(struct User u) {
    static char password[50];
    FILE *file = fopen(USERS, "r");
    if (file == NULL) {
        perror("Failed to open users file");
        return NULL;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char stored_name[50], stored_password[50];
        sscanf(line, "%*d %s %s", stored_name, stored_password);
        if (strcmp(stored_name, u.name) == 0) {
            strcpy(password, stored_password);
            fclose(file);
            return password;
        }
    }
    fclose(file);
    return NULL;
}
