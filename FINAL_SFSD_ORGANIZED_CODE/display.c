#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include"primitives.h"
#include"Index.h"


void DISPLAY_HEADER(){
    header H = readHeader();
    
    printf("\n==== HEADER ====\n");
    printf("[%d %d %d %d]",H.FIRSTB,H.LASTB,H.FREE_POSITION_B,H.DELETED);
    printf("\n================\n");

}
void displayEtudiants(const Etudiant* etudiants, int nbEtudiants) {
    for (int i = 0; i < nbEtudiants; i++) {
        printf("\t\tID: %d | Nom: %s | Matricule: %s | Deleted: %d\n", etudiants[i].id, etudiants[i].nom, etudiants[i].mat, etudiants[i].deleted);
    }
}
void displayBloc(Bloc b){
    
    printf("\ttab: %s\n",b.tab);
    Etudiant* myEtudiants = convertBE(&b);
    displayEtudiants(myEtudiants, b.nb);
    printf("\tnb: %d - suivant: %d\n",b.nb,b.sv);
    
}

bool verifyChars(const char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isalpha((unsigned char)str[i]) && str[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool verifyDigits(const char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

char *readLine(int size, const char *message, int cond) {
    char *s = (char *)calloc(size, sizeof(char));

    switch (cond) {
        case 1:
            do {
                printf("%s", message);
                fflush(stdout);  // Flush the output buffer to ensure the message is displayed
                fgets(s, size, stdin);
                s[strcspn(s, "\n")] = '\0';  // Remove the newline character
            } while (!verifyDigits(s));

            return s;
        case 2:
            do {
                printf("%s", message);
                fflush(stdout);  // Flush the output buffer to ensure the message is displayed
                fgets(s, size, stdin);
                s[strcspn(s, "\n")] = '\0';  // Remove the newline character
            } while (!verifyChars(s));

            return s;
        default:
            printf("%s", message);
            fflush(stdout);  // Flush the output buffer to ensure the message is displayed
            fgets(s, size, stdin);
            s[strcspn(s, "\n")] = '\0';  // Remove the newline character
            return s;
    }
}

void addByConsole(){
    Etudiant etu;
        
        etu.deleted=0;
        
        char *nom = readLine(NAME_SIZE,"\t\tEnter Fullname : ",2);
        printf("\t\t\tYou Entered %s\n",nom);
        
        char *mat = readLine(MAT_SIZE,"\t\tEnter Matricule : ",1);
        printf("\t\t\tYou Entered %s\n",mat);
        //getchar();
        int cle;
        printf("\n\t\t Enter id [ 0 < id < 1000 ]: ");
        scanf("%d", &cle);
        while(UsedId(cle)){
            printf("\n\t\t The id[%d]: is Already Used try another one: ",cle);
            scanf("%d", &cle);
        }
        
        printf("\t\t\tId[%d] is Accepted :)\n",cle);
        
        strcpy(etu.nom,nom);
        etu.id=cle;
        strcpy(etu.mat,mat);
        
        free(nom);
        free(mat);
        
        insertEtudiant(etu);
}
void removeByConsole(){
    int cle;
    printf("\t\tEnter Student's id To remove :");
    scanf("%d",&cle);
    
    removeEtudiant(cle);
}

void displayByConsole(){
    int cle;
    printf("\t\tEnter Student Id : ");
    scanf("%d",&cle);
    
    SearchInfo SE = Search(cle);
    
    if(!SE.trouv){
        printf("NotFound!");
        return;
    }
    if((SE.etu).deleted){
        printf("\t\t => The Student Was Deleted!\n");
        return;
    }
    printf("\t\t  [#]. Student Name : %s\n",(SE.etu).nom);
    printf("\t\t  [#]. Student Matricule : %s\n",(SE.etu).mat);
    printf("\t\t  [#]. Student id : %d\n",(SE.etu).id);
}
void displayAllByConsole(){
    
    IndexElement * IEpTr = readIndex();
    int nb = getIndexHeader();
    bubbleSort(IEpTr,nb);
    
    printf("\t\t************************__ALL_STUDENTS__************************\n");
    printf("\t\t----------------------------------------------------------------\n");
    printf("\t\t        Id             FullName                 Matricule \n");
    printf("\t\t----------------------------------------------------------------\n");
    int count=0;
    int deletedCount=0;
    while(count < nb){
        Etudiant etu = readStudent(IEpTr[count].i,IEpTr[count].j);
        if(etu.deleted==0){
            printf("\t\t        %-3d          %-25s   %-12s\n", etu.id, etu.nom, etu.mat);
        }
        else{
            deletedCount++;
        }
        
        count++;
    }
    printf("\t\t  [#Total : %d]\n",nb-deletedCount);
    printf("\t\t****************************************************************\n");
    
}

void printAllInFile(){
    //char path[]="C:/Users/LENOVO/OneDrive/Bureau/FINAL_SFSD_CODE/SFSD_TEXTS";
    char *fileName = readLine(100,"\t\tEnter Text File Name : ",3);
    strcat(fileName,".txt");
    FILE * PRINT =fopen(fileName,"w");
    if(PRINT==NULL){
        return;
    }
    IndexElement * IEpTr = readIndex();
    int nb = getIndexHeader();
    bubbleSort(IEpTr,nb);
    
    fputs("\t****************************__ALL_STUDENTS__****************************\n",PRINT);
    fputs("\t\t----------------------------------------------------------------\n",PRINT);
    fputs("\t\t        Id           FullName                    Matricule \n",PRINT);
    fputs("\t\t----------------------------------------------------------------\n",PRINT);

    int count=0;
    int deletedCount=0;

    while(count < nb){
        Etudiant etu = readStudent(IEpTr[count].i,IEpTr[count].j);
        if(etu.deleted==0){
            char *Line=(char*)calloc(100,sizeof(char));
            sprintf(Line,"\t\t        %-3d          %-25s   %-12s\n", etu.id, etu.nom, etu.mat);
            fputs(Line,PRINT);

            free(Line);
        }
        else{
            deletedCount++;
        }
        count++;
    }
    fputs("\t\t----------------------------------------------------------------\n",PRINT);
    char * end =(char*)calloc(100,sizeof(char));
    sprintf(end,"\t\t  [#Total : %d]\n",nb-deletedCount);
    fputs(end,PRINT);
    free(end);
    fputs("\t************************************************************************\n",PRINT);
    printf("\tStudents Printed Succefully to the file[%s]\n",fileName);
    fclose(PRINT);
}