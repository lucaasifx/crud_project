#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crud.h"
#include "rotins.h"
#define STRTAM 51

int main(int argc, char *argv[]) {
    // garante que a seed de geração da seed será sempre aleatória
    srand(time(NULL));

    centralizedMessage("CRUD PROJECT V1");
    List *usersList = createList();

    // alimenta a lista se existir algum usuário já cadastrado
    FILE *dataB;
    recoveryDB(usersList, dataB);


    int opc;
    while(true) {
        menu();
        printf("Your option: ");
        scanf("%d", &opc);
        getchar();

        switch(opc) {
            case 1:{
                centralizedMessage("List of Users");
                displayList(usersList);
                puts("");
                break;
            }
            case 2: {
                centralizedMessage("Add user");
                char name[STRTAM], 
                    email[STRTAM],
                    password[STRTAM];
                readField("Name", name);
                readField("Email", email);
                readField("Password", password);
                append(usersList, name, email, password);
                puts("");
                break;
            }

            case 3: {
                int opc2;
                char cmp[STRTAM];
                centralizedMessage("Change user fields");
                readField("User to update", cmp);
                Node *q = query(usersList, cmp);
                if(q) {
                    updateMenu();
                    printf("Option to change: ");
                    scanf("%d", &opc2);
                    getchar();

                    switch (opc2) {
                        case 1: {
                            char name[STRTAM];
                            readField("Name", name);
                            updateName(usersList, q, name);
                            break;
                        }
                        case 2: {
                            char email[STRTAM];
                            readField("Email", email);
                            updateEmail(q, email);
                            break;
                        }
                        case 3: {
                            char password[STRTAM];
                            readField("Password", password);
                            updatePassword(q, password);                            
                            break;
                        }
                        default: {
                            puts("Invalid option!");
                            break;
                        }
                    }
                }
                else
                    printf("%s isn't in list.\n", cmp);
                break;
            }
            case 4: {
                centralizedMessage("Remove User");
                char name[STRTAM];
                readField("Name", name);
                delUser(usersList, name);
                break;
            }
            case 5:
                break;
            default:
                puts("Invalid option!");
        }
        if(opc == 5)
            break;
    }

    if(!emptyList(usersList))
        backupDB(usersList, dataB);
    deallocList(&usersList);
    return 0;
}
