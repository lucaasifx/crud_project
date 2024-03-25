#include "rotins.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "crud.h"
#define STRTAM 51

void centralizedMessage(const char *msg) {
    int centeralign = strlen(msg) + 32;
    char adornment[centeralign];
    for(int i = 0; i < centeralign; i++) {
        if(i % 2)
            adornment[i] = '=';
        else
            adornment[i] = '-';    
    }
    adornment[centeralign] = '\0';
    puts(adornment);
    printf("                %s\n", msg);
    puts(adornment);
}

void menu() {
    puts("Operations:");
    puts("\t\t[1] List users");
    puts("\t\t[2] Add new user");
    puts("\t\t[3] Change user fields");
    puts("\t\t[4] Remove user");
    puts("\t\t[5] Exit");
}
void readField(const char *nfield, char *field) {
    do {
        printf("%s: ", nfield);
        scanf("%50[^\n]", field);
        getchar();
    } while(!verifyText(field));
}

void recoveryDB(List *headList, FILE *dataB) {
    while(true) {
        dataB = fopen("./data/dataB.txt", "r");
        if(dataB)
            break;
        dataB = fopen("./data/dataB.txt", "w");
        if(!dataB) {
            perror("Fatal error");
            exit(1);
        }
        else {
            fclose(dataB);
            return;
        }
    }

    char name[STRTAM],
        emailUs[STRTAM], 
        passUs[STRTAM], 
        key[20];

    while (fgets(name, STRTAM, dataB)) {
        fgets(emailUs, STRTAM, dataB);
        fgets(passUs, STRTAM, dataB);
        fgets(key, 20, dataB);
        name[strcspn(name, "\n")] = '\0';
        emailUs[strcspn(emailUs, "\n")] = '\0';
        passUs[strcspn(passUs, "\n")] = '\0';
        key[strcspn(key, "\n")] = '\0';
        fappend(headList, name, emailUs, passUs, atoi(key));
    }
    fclose(dataB);
}




// no futuro uma verificação muito mais robusta!
// atualmente verifica apenas se a string é vazia
bool verifyText(const char *data) {
    return strcmp(data, "") != 0;
}

void updateMenu() {
    puts("Change options: ");
    puts("\t\t[1] Change name");
    puts("\t\t[2] Change email");
    puts("\t\t[3] Change password");
}

