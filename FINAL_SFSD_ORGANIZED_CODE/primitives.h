#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#define SIZE 200
#define MAT_SIZE 12
#define NAME_SIZE 40

#include<stdbool.h>

typedef struct {
    char tab[SIZE];
    int nb;
    int sv;
} Bloc;

typedef struct {
    char mat[MAT_SIZE];
    int id;
    char nom[NAME_SIZE];
    int deleted;
} Etudiant;

typedef struct {
    int FIRSTB;
    int LASTB;
    int FREE_POSITION_B;
    int DELETED;
} header;

typedef struct {
    Etudiant etu;
    int trouv;
    int i;
    int j;
} SearchInfo;

// Function Declarations
void CREATE_HEADER(int FIRSTB, int LASTB, int FREE_POSITION_B,int DELETED);
header readHeader();
int HEADER(int INDEX);
void SET_HEADER(int INDEX, int val);

void AddToBloc(char s[SIZE],char str[60]);
void PRINTIN(char *result,int n,char mat[],char nom[],int del);

void addBloc(Bloc *b);
Bloc readBloc(int num);

bool UsedId(int cle);

Etudiant readStudent(int i,int j);
void insertEtudiant(Etudiant etu);

Etudiant *convertBE(Bloc *bloc);

SearchInfo Search(int c);
void removeEtudiant(int c);

#endif // PRIMITIVE_H
