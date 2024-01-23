#ifndef DISPLAY_H
#define DISPLAY_H
#include"primitives.h"

void displayEtudiants(const Etudiant* etudiants, int nbEtudiants);
void DISPLAY_HEADER();
void displayBloc(Bloc b);
bool verifyChars(const char *str);
bool verifyDigits(const char *str);
char *readLine(int size, const char *message, int cond);
void addByConsole();
void removeByConsole();
void displayByConsole();
void displayAllByConsole();
void printAllInFile();

#endif // DISPLAY_H