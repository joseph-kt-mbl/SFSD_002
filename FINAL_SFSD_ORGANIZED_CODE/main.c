#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"primitives.h"
#include"index.h"
#include"display.h"

int main(){   
    char *nom1="Abdellaoui Youcef";
    char *nom2="Ghaoui Wail";
    char *nom3="Naili Walid";
    char *mat1="222231609707";
    char *mat2="222231589020";
    char *mat3="222231412409";
    
    printf("\n\t\t------------------------------------------------------------\n");
    printf("\n\t\t\t\tWelcome To SFSD Project \n");
    printf("\t\t\tING-INFO-2/SEC-[A]/GRP-[1]/GRP-SFSD-02.\n");
    printf("\n\t\t------------------------------------------------------------\n");
    printf("\t\t\t      Nom                   Matricule     \n\n");
    printf("\t\t\t%-25s  %-12s\n",nom1,mat1);
    printf("\t\t\t%-25s  %-12s\n",nom2,mat2);
    printf("\t\t\t%-25s  %-12s\n",nom3,mat3);
    printf("\t\t=============================================================\n\n");
    
    int c;
    printf("\t\t  If you Wanna create The file Enter 1 ,Quit Enter 0 : ");
    scanf("%d",&c);
    if(!c){
        return 0;
    }
    
    CREATE_HEADER(0,0,0,0);
    createIndex(0);
    
    printf("\t\t  The file [%s] is Created Successfuly :)\n","Bloc.bin");
    printf(" ");
    int op;
    int q=1;
    while(q){
        printf("\t\t  Options:\n");
        printf("\t\t\t1. Add Student\n");
        printf("\t\t\t2. Remove Student\n");
        printf("\t\t\t3. Display Student\n");
        printf("\t\t\t4. Display All Students\n");
        printf("\t\t\t5. Print All in File");
        printf("\t\t\t6. Quit\n");
        printf("\t\tYour Option Here: ");
        scanf("%d",&op);
        getchar();
        
        switch(op){
            case 1: addByConsole();
                break;
            case 2: removeByConsole();
                break;
            case 3: displayByConsole();
                break;
            case 4: displayAllByConsole();
                break;
            case 5: printAllInFile();
            default:return 0;
        }
        
        printf("\t\t[GoBack to options:1, Quit:0] : ");
        scanf("%d",&q);
    }
    
    return 0;
}