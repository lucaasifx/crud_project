#ifndef CRUD_H
#define CRUD_H

#include <stdbool.h>
#include <stdio.h>

typedef struct user User;
typedef struct node Node;
typedef struct list List;

List *createList();
Node *query(const List *, const char *);
bool emptyList(const List *);
void append(List *, const char *, const char *, const char *);
void updateName(List *, Node *, const char *);
void updateEmail(Node *, const char *);
void updatePassword(Node *, const char *);
void delUser(List *, const char *);
void deallocList(List **);
void displayList(const List *);
void backupDB(const List *, FILE *);


Node *fcreateNode(const char *, const char *, const char *, const int);
void fappend(List *, const char *, const char *, const char *, const int);

#endif