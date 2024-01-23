#include<stdio.h>
#include"primitives.h"
#include"index.h"
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

char fileName[] = "Blocs.bin";
//Create Header
void CREATE_HEADER(int FIRSTB, int LASTB, int FREE_POSITION_B,int DELETED){
    
    FILE * verify =fopen(fileName,"rb");
    header h;
    if(verify!=NULL){
        fclose(verify);
        return;// return if the header exist.
    }
    FILE * w =fopen(fileName,"wb");
    

    header H;
    H.FIRSTB=FIRSTB;
    H.LASTB=LASTB;
    H.FREE_POSITION_B=FREE_POSITION_B;
    H.DELETED=DELETED;
    
    fwrite(&H,sizeof(H),1,w);
    
    fclose(w);
}
//Load Header to Buffer.
header readHeader(){
    
    FILE * RH = fopen(fileName,"rb");
     header H;
    if (RH== NULL) {
        printf("Unable to open the file.\n");
        return H;
    }
   
    fread(&H,sizeof(H),1,RH);
    fclose(RH);
    
    return H;
}
//Get One Header Value
int HEADER(int INDEX){
    
    FILE * f =fopen(fileName,"rb");
    header H;
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        fclose(f);
        return -111;        //value of null
    }
    fread(&H, sizeof(H), 1, f);    
        
    switch(INDEX){
        case 1: 
                fclose(f);
                return H.FIRSTB;
                break;
        case 2: 
                fclose(f);
                return H.LASTB;
                break;
                
        case 3:
                fclose(f);
                return H.FREE_POSITION_B;
                break;
        case 4:
                fclose(f);
                return H.DELETED;
                break;
        default:
            fclose(f);
            return -222;    //INDEX OverFlow Value
    }
    fclose(f);
}
//Set Header
void SET_HEADER(int INDEX , int val){
    
    header H = readHeader();
    
    FILE * writer = fopen(fileName,"rb+");
    
    switch(INDEX){
        case 1: 
                H.FIRSTB=val;
                fwrite(&H,sizeof(header),1,writer);
                fclose(writer);
                return;
    
        case 2: 
        
                H.LASTB=val;
                fwrite(&H,sizeof(header),1,writer);
                fclose(writer);
                return;
                
        case 3:
            
                H.FREE_POSITION_B=val;
                fwrite(&H,sizeof(header),1,writer);
                fclose(writer);
                return;
                
        case 4:
                
                H.DELETED=val;
                fwrite(&H,sizeof(header),1,writer);
                fclose(writer);
                return;
        default:
            printf("INDEX NOT VALID [FROM SET_HEADER]");
            return;
    }
}

//primitives used on Incapsulation.
void AddToBloc(char s[SIZE],char str[60]){
    
    //s+=s+str
    size_t current_length = strlen(s);
    
    if (current_length + strlen(str) < SIZE){
        if (s[0] != '\0') {
            strcat(s, str);
        } else {
            strcpy(s, str);
        }
    }
    else{
        printf("Error: Adding string exceeds buffer size.\n");
    }
}
void PRINTIN(char *result,int n,char mat[],char nom[],int del){

    
    sprintf(result,"%d*",n);                //n*
    sprintf(result,"%s%d#",result,del);     //n*deleted#
    strncat(result,mat,12);                 //n*deleted#mat
    strncat(result,"#",2);                  //n*deleted#mat#
    strncat(result,nom,40);                 //n*deleted#mat#nom
    strncat(result,"$",2);                  //n*deleted#mat#nom$
    
}

//add And Read Blocs
void addBloc(Bloc *b){
    FILE * writer = fopen(fileName,"ab");
    
    if(writer==NULL){
        printf("Unable to open the file[from addBloc].\n");
        return;
    }
    fwrite(b,sizeof(Bloc),1,writer);//b is a pointer[contain address].
    fclose(writer);
    
    SET_HEADER(2,HEADER(2)+1);//increment number of Blocs.
}
Bloc readBloc(int num){
    
    FILE * reader=fopen(fileName,"rb");
    Bloc b;
    if(reader==NULL){
        printf("Unable to open the file[form readBloc].\n");
        return b;
    }
    ///fichier=header+[1,2,3,4]
    fseek(reader,sizeof(header)+(num-1)*sizeof(Bloc),SEEK_SET);

    if(fread(&b,sizeof(Bloc),1,reader)){
        //printf("Bloc Loaded Successfuly!\n");
    }
    else{printf("Precess Of Loading Bloc Failed!");}
    fclose(reader);
    return b;
}

//read a student by using index of bloc and index of student.
Etudiant readStudent(int i,int j){
    Bloc bloc = readBloc(i);

        char* token = strtok(bloc.tab, "$");//recuperer premeier Etudiant.
        
        int count = 0;
        Etudiant etu;

        while (count<j) {
            //printf("\n\t\ttoken[%d] => %s \n",count,token);
            token = strtok(NULL, "$");// recuperer : deuxiemme etudiant .
            count++;
        }

        sscanf(token, "%d*%d#%[^#]#%[^$]",
                    &etu.id, &etu.deleted,etu.mat,etu.nom);


            char str[3];// max-len 3[023]
            int x=0;
            
            while(token[x]!='*'){
                str[x]=token[x];
                x++;
            }
            
            char idStr[x];//2
            int y=0;
            while(y<x){
                idStr[y]=str[y];
                y++;
            }
            
            etu.id = atoi(idStr);
            
            
            return etu;
}

//Confirm That the Id=cle is unic.
bool UsedId(int cle){
    IndexElement * IEpTr = readIndex();
    int ind = SearchForId(IEpTr,0,getIndexHeader()-1,cle);
    
    return ind==-1?false:true;
}

//insert a student to the file.
void insertEtudiant(Etudiant etu){
    
    if(UsedId(etu.id)){
        return;
    }
    
    // Read the header
    header fileHeader = readHeader();
    char result[60];
    PRINTIN(result, etu.id, etu.mat, etu.nom, etu.deleted);
    
    // Read the last block if exist
    IndexElement IE;
    IE.cle=etu.id;
    
    if(fileHeader.LASTB!=0){
        Bloc BUFFER = readBloc(fileHeader.LASTB);
    

        // Calculate the available space in the last block, considering separators
        int a = strlen(BUFFER.tab) + (BUFFER.nb - 1) * 2;  // Each '$' adds 1 to length, and there are (nb - 1) separators
        

        // Searching For Free Position.
        if (SIZE - a >= 60){
        
            strcat(BUFFER.tab, result);
            BUFFER.nb++;

            // Write the updated block back to the file
            FILE* WRITER = fopen(fileName, "rb+");
            fseek(WRITER, sizeof(header) + sizeof(Bloc) * (fileHeader.LASTB - 1), SEEK_SET);
            fwrite(&BUFFER, sizeof(Bloc), 1, WRITER);
            fclose(WRITER);

            IE.i=fileHeader.LASTB;
            IE.j=BUFFER.nb-1;

            //No need to Update the header
        }
        else {
        // Create a new block
        Bloc NEW_BLOCK;
        NEW_BLOCK.nb = 1;
        NEW_BLOCK.sv = -1;
        strcpy(NEW_BLOCK.tab, result);

        // Write the new block to the file
        addBloc(&NEW_BLOCK);

        IE.i=fileHeader.LASTB+1;
        IE.j=0;

        // Update the header
        SET_HEADER(2, fileHeader.LASTB + 1);
        }
        
    }
    else {
        // Create a new block
        Bloc NEW_BLOCK;
        NEW_BLOCK.nb = 1;
        NEW_BLOCK.sv = -1;
        strcpy(NEW_BLOCK.tab, result);

        // Write the new block to the file
        addBloc(&NEW_BLOCK);
        IE.i=1;
        IE.j=0;


        // Update the header
        SET_HEADER(2, fileHeader.LASTB + 1);
    }

    addElementToIndex(IE.cle,IE.i,IE.j);
    
}

//Return Array Of Etudiant from one Bloc.
Etudiant* convertBE(Bloc* bloc) {
    
    Etudiant* etudiants = (Etudiant*)malloc(bloc->nb * sizeof(Etudiant));
    //allocate bloc->nb places of Etudiant struct.

    if (etudiants == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    //tab = [etudiant]$[etudiant]$[etudiant]$
    char* token = strtok(bloc->tab, "$");//recuperer premeier Etudiant.
    int count = 0;

    while (token != NULL) {
        // printf("Token: [%s]\n", token);
        sscanf(token, "%d*%d#%[^#]#%[^$]",
                &etudiants[count].id, &etudiants[count].deleted,
                etudiants[count].mat, etudiants[count].nom);

        char str[3];// max-len 3
        int i=0;
        while(token[i]!='*'){
            str[i]=token[i];
            i++;
        }
        // printf("{%s}\n",str);
        sscanf(str,"%d",&(etudiants[count].id));
        token = strtok(NULL, "$");// recuperer : deuxiemme etudiant .
        count++;
    }
    bloc->nb = count;
    return etudiants;
}

//Search for student with id=cle.
SearchInfo Search(int cle){
    IndexElement * IEpTr = readIndex();
    int ind = SearchForId(IEpTr,0,getIndexHeader()-1,cle);
    
    if(ind!=-1){
        IndexElement IE = IEpTr[ind];
        Etudiant etu = readStudent(IE.i,IE.j);

            SearchInfo SE;
            SE.etu=etu;
            SE.trouv=true;
            SE.i=IE.i;
            SE.j=IE.j;


            return SE;
    }
    printf("cle : %d does not exist",cle);
     SearchInfo SE;
     SE.trouv=false;

     return SE;
}

//remove logically the student with id=c from the file.
void removeEtudiant(int c){
    SearchInfo result = Search(c);
    
    if (!result.trouv) {
        printf("\t\tStudent Not Found,it can't be removed!\n");
        return;
    }
    if((result.etu).deleted){
        printf("\t\tThe Student With id[%d] is Already Removed!",c);
        return;
    }
    printf("\t\t[*]Search result for ID %d: trouv=%d, i=%d, j=%d\n", c, result.trouv, result.i, result.j);
    Bloc BUFFER = readBloc(result.i);

    int cpt = 0;
    int a = 0;

    while (a < strlen(BUFFER.tab)) {
        if (BUFFER.tab[a] == '$') {
            cpt++;
        }
        if (cpt == result.j) {
            break;
        }
        a++;
    }

    while (BUFFER.tab[a] != '*') {
        a++;
    }

    // Debug print
    //printf("Before removal: BUFFER.tab[a+1] = %c\n", BUFFER.tab[a + 1]);

    // Modify the deletion flag
    BUFFER.tab[a + 1] = '1';

    // Debug print
    //printf("After removal: BUFFER.tab[a+1] = %c\n", BUFFER.tab[a + 1]);

    FILE *writer = fopen(fileName, "rb+");
    fseek(writer, sizeof(header) + (result.i - 1) * sizeof(Bloc), SEEK_SET);
    fwrite(&BUFFER, sizeof(Bloc), 1, writer);
    fclose(writer);

    // Debug print
    printf("\t\t => Student[%d] removed successfully\n",c);


}

