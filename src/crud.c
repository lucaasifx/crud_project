#include "crud.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

#define STRtam 51


typedef struct user {
    char name[STRtam];
    char email[STRtam];
    char password[STRtam];
} User;

typedef struct node {
    User *user;
    struct node *prev, *next;
    int key;
} Node;

typedef struct list {
    Node *head;
    Node *tail;
    int quantNodes; 
} List;


User *createUser(const char *name, const char *email, const char *password) {
    User *u = (User *) malloc(sizeof(User));
    if(!u) {
        puts("Allocation error!");
        exit(1);
    }

    strcpy(u->name, name);
    strcpy(u->email, email);
    strcpy(u->password, password);

    return u;
}

List *createList() {
    // List *l = (List *) calloc(1, sizeof(List));
    List *l = (List *) malloc(sizeof(List));
    l->head = NULL;
    l->tail = NULL;
    l->quantNodes = 0;

    return l;
}

bool emptyList(const List *headList) {
    return !headList->quantNodes;
}

bool keyinList(const List *headList, const int key) {
    Node *backup = headList->head;
    while(backup && key != backup->key) {
        backup = backup->next;
    }
    return backup ? true : false;
}

int hashGen(const List *headList) {
    int gen;
    while(true) {
        gen = keyGenerate();
        if(!keyinList(headList, gen))
            break;
    }
    return gen;
}

Node *createNode(const List *headList, const char *name, const char *email, const char *password) {
    User *u = createUser(name, email, password);
    Node *n = (Node *) malloc(sizeof(Node));
    if(!n) {
        puts("Allocation error!");
        exit(1);
    }
    n->next = NULL;
    n->prev = NULL;
    n->user = u;
    n->key = hashGen(headList);

    return n;
}


// void organizeList(List *headList) {
//     if(emptyList(headList))
//         return;
//     if(headList->head != headList->tail) {
//         Node *backup = headList->tail, *aux = backup->prev;
//         while(aux && strcmp(backup->user->name, aux->user->name) < 0) {
//             // salva a referência dos nós antecessores
//             backup->prev = aux->prev;
//             // salva a referência dos nós sucessores
//             aux->prev = backup;

//             // correção do encadeamento:
//             aux->next = backup->next;
//             backup->next = aux;
//             if(backup->prev)
//                 backup->prev->next = backup;
//             else
//                 headList->head = backup;
//             if(aux->next)
//                 aux->next->prev = aux;
//             else
//                 headList->tail = aux;
            
//             // nó anterior agora é o backup 
//             aux = backup->prev;
//         }
//     }
// }

// // Lógica da função de inserção
//     // Inserção na calda da lista é trivial
//     // Insere no fim e depois, se necessário, organiza a lista.
// void append(List *headList, char *name, char *email, char *password) {
//     Node *n = createNode(headList, name, email, password);
//     n->prev = headList->tail;
//     if(emptyList(headList))
//         headList->head = n;
//     else
//         headList->tail->next = n;
//     headList->tail = n;
//     headList->quantNodes++;
//     organizeList(headList);
// }

void append(List *headList, const char *name, const char *email, const char *password) {
    Node *n = createNode(headList, name, email, password);
    if(emptyList(headList)) {
        headList->head = n;
        headList->tail = n;
    }
    else if(strcmp(name, headList->head->user->name) < 0) {
        n->next = headList->head;
        headList->head->prev = n;
        headList->head = n;
    }
    else if(strcmp(name, headList->tail->user->name) > 0) {
        n->prev = headList->tail;
        headList->tail->next = n;
        headList->tail = n;
    }
    else {
        Node *backup = headList->head;
        while(backup && strcmp(name, backup->user->name) > 0)
            backup = backup->next;
        n->prev = backup->prev;
        n->next = backup;
        backup->prev->next = n;
        backup->prev = n;
    }
    headList->quantNodes++;
}


Node *query(const List *headList, const char *name) {
    if(!emptyList(headList)) {
        Node *backup = headList->head;
        while(backup && strcmp(name, backup->user->name))
            backup = backup->next;
        if(backup)
            return backup;
    }
    return NULL;
}

Node *copyNode(Node *origin) {
    Node *cp = (Node *) malloc(sizeof(Node));
    if(!cp) {
        puts("Allocation error!");
        exit(1);
    }
    User *u = createUser(origin->user->name, origin->user->email, origin->user->password);
    cp->next = NULL;
    cp->prev = NULL;
    cp->user = u;
    cp->key = origin->key;
    return cp;
}

void updateName(List *headList, Node *node, const char * name) {
    // cria uma cópia perfeita do antigo nó
    // deleta o antigo, adiciona o novo no lugar correto da lista
    // pq não usar a função append? Simplesmente pq não quero que
        // a ID mude, uma vez que a intenção da função é apenas
        // corrigir um possível erro de digitação no nome do usuário
        // Exemplo: Lucas Emanuel ao invés de Lucas Emanoel
    Node *cp = copyNode(node);
    strcpy(cp->user->name, name);
    // remove o antigo nó e decrementa quantNodes
    delUser(headList, node->user->name);
    if(emptyList(headList)) {
        headList->head = cp;
        headList->tail = cp;
    }
    else if(strcmp(name, headList->head->user->name) < 0) {
        cp->next = headList->head;
        headList->head->prev = cp;
        headList->head = cp;
    }
    else if(strcmp(name, headList->tail->user->name) > 0) {
        cp->prev = headList->tail;
        headList->tail->next = cp;
        headList->tail = cp;
    }
    else {
        Node *backup = headList->head;
        while(backup && strcmp(name, backup->user->name) > 0)
            backup = backup->next;
        cp->prev = backup->prev;
        cp->next = backup;
        backup->prev->next = cp;
        backup->prev = cp;
    }
    // incrementa quantNodes para corrigir a remoção
    headList->quantNodes++;
}

void deallocNode(Node **n) {
    Node *node = *n;
    free(node);
    *n = NULL;
}

void delUser(List *headList, const char *name) {
    if(emptyList(headList))
        return;
    else {
        Node *backup = headList->head, *prev = NULL;
        while(backup && strcmp(backup->user->name, name)) {
            prev = backup;
            backup = backup->next;
        }
        if(backup) {
            if(backup == headList->head) {
                headList->head = backup->next;
                if(headList->head)
                    headList->head->prev = NULL;
                else
                    headList->tail = NULL;
            }
            else {
                prev->next = backup->next;
                if(!prev->next)
                    headList->tail = prev;
                else
                    prev->next->prev = prev;
            }
            deallocNode(&backup);
            headList->quantNodes--;
        }
        else
            printf("%s isn't in list\n", name);
    }
}

void updateEmail(Node *node, const char *email) {
    strcpy(node->user->email, email);
}
void updatePassword(Node *node, const char *password) {
    strcpy(node->user->password, password);
}

void deallocUser(User **u) {
    User *us = *u;
    free(us);
    *u = NULL;
}



void displayList(const List *headList) {
    if(emptyList(headList))
        puts("List is empty.");
    else {
        Node *backup = headList->head;
        int quant = headList->quantNodes;
        for(int i = 0; i < quant; i++) {
            printf("Nº: %d|ID: %d|Name: %s|Email: %s|Password: %s|\n", i + 1, 
                                                                    backup->key,
                                                                    backup->user->name,
                                                                    backup->user->email,
                                                                    backup->user->password);
            backup = backup->next;
        }
    }
}


void deallocList(List **l) {
    List *list = *l;
    Node *backup = list->head, *aux = NULL;
    while(backup) {
        aux = backup;
        backup = backup->next;
        deallocNode(&aux);
    }
    free(list);
    *l = NULL;
}


// FUNÇOES RELACIONADAS A ARQUIVOS
Node *fcreateNode(const char *name, const char *email, 
const char *password, const int key) {
    User *u = createUser(name, email, password);
    Node *n = (Node *) malloc(sizeof(Node));
    if(!n) {
        puts("Allocation error!");
        exit(1);
    }
    n->next = NULL;
    n->prev = NULL;
    n->user = u;
    n->key = key;

    return n;
}

void fappend(List *headList, const char *name, const char *email, 
const char *password, const int key) {
    Node *n = fcreateNode(name, email, password, key);
    if(emptyList(headList)) {
        headList->head = n;
        headList->tail = n;
    }
    else if(strcmp(name, headList->head->user->name) < 0) {
        n->next = headList->head;
        headList->head->prev = n;
        headList->head = n;
    }
    else if(strcmp(name, headList->tail->user->name) > 0) {
        n->prev = headList->tail;
        headList->tail->next = n;
        headList->tail = n;
    }
    else {
        Node *backup = headList->head;
        while(backup && strcmp(name, backup->user->name) > 0)
            backup = backup->next;
        n->prev = backup->prev;
        n->next = backup;
        backup->prev->next = n;
        backup->prev = n;
    }
    headList->quantNodes++;
}

// escreve os nós atuais no arquivo
void backupDB(const List *headList, FILE *dataB) {
    dataB = fopen("./data/dataB.txt", "w");
    if(dataB) {
        Node *backup = headList->head;
        while(backup) {
            fprintf(dataB, "%s\n", backup->user->name);
            fprintf(dataB, "%s\n", backup->user->email);
            fprintf(dataB, "%s\n", backup->user->password);
            fprintf(dataB, "%d\n", backup->key);
            backup = backup->next;
        }
        fclose(dataB);
    }
    else {
        perror("Fatal error");
        exit(1);
    }
}

