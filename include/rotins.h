#ifndef ROTINS_H
#define ROTINS_H

#include <stdio.h>
#include <stdbool.h>
#include <crud.h>

#define STRTAM 51

bool verifyText(const char *);
void centralizedMessage(const char *);
void menu();
void readField(const char *, char *);
void recoveryDB(List *, FILE *);
void updateMenu();

#endif
