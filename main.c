#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define SIZE 200
#define MAT_SIZE 12
#define NAME_SIZE 40

char fileName[]="Blocs.bin";
char indexFileName[]="Index.bin"; 

//index records
typedef struct{
    int nb;
}IndexHeader;
typedef struct{
    int cle;
    int i;
    int j;
}IndexElement;
//index records end.

typedef struct {
    char tab[SIZE];
    int nb;
    int sv;
}Bloc;
typedef struct {
    char mat[MAT_SIZE];
    int id;
    char nom[NAME_SIZE];
    int deleted;
}Etudiant;
typedef struct{
    int FIRSTB;
    int LASTB;
    int FREE_POSITION_B;
    int DELETED;
}header;

typedef struct{
    Etudiant etu;
    bool trouv;
    int i;
    int j;
}SearchInfo;



void CREATE_HEADER(int FIRSTB, int LASTB, int FREE_POSITION_B,int DELETED);
void DISPLAY_HEADER();
int HEADER(int INDEX);
void addBloc(Bloc * b);
void AddToBloc(char *s,char str[60]);
Bloc readBloc();
void displayBloc(Bloc b);
Etudiant* convertBE(Bloc* bloc);
void displayEtudiants(const Etudiant* etudiants, int nbEtudiants);
void createIndex(int nb);
void setIndexHeader(int nb);
void addElementToIndex(int cle, int i, int j);
int getIndexHeader();
int SearchForId(IndexElement array[], int left, int right, int cle);
void PRINTIN(char *result,int n,char mat[],char nom[],int del);
void sortIndexFile();
void bubbleSort(IndexElement arr[], int n);
void swap(IndexElement *xp, IndexElement *yp);
Etudiant readStudent(int i,int j);
SearchInfo Search(int cle);


int main ()
{   
    CREATE_HEADER(10,20,30,40); // create File and put header
    DISPLAY_HEADER();// Confirm That The File Is Created and Header Puted. 
    
        int n=3;
        Etudiant etu[n];
        
        for(int i=0;i<n;i++){
            etu[i].deleted=1;
        }
        
        
        
        strcpy(etu[0].mat,"222231600795");
        etu[0].id=44;
        strcpy(etu[0].nom,"ABDELLAOUI_YOUCEF");
        
        strcpy(etu[1].mat,"222231609808");
        etu[1].id=44;
        strcpy(etu[1].nom,"WAIL_GHAOUI");
        
        strcpy(etu[2].mat,"222231609909");
        etu[2].id=44;
        strcpy(etu[2].nom,"NAILI_WALID");
        
        
        
        
        Bloc etuBloc;//My Buffer 
        etuBloc.nb=n;
    

        char result[3][60];
        
        //3+1+1+1+12+1+40+1=60   
        
        PRINTIN(result[0],etu[0].id,etu[0].mat,etu[0].nom,etu[0].deleted);
        PRINTIN(result[1],etu[1].id,etu[1].mat,etu[1].nom,etu[1].deleted);
        PRINTIN(result[2],etu[2].id,etu[2].mat,etu[2].nom,etu[2].deleted);

        
        AddToBloc(etuBloc.tab,result[0]);
        AddToBloc(etuBloc.tab,result[1]);
        AddToBloc(etuBloc.tab,result[2]);
        

        printf("%s ", etuBloc.tab); // fseek(fichier,40,SEEK_SET);
        
        addBloc(&etuBloc);
        
        printf("\n\n=========================================\n");

        Bloc Buffer = readBloc(4);//read the first Bloc from the File.
        
        displayBloc(Buffer);
        
        printf("\n\n=========================================\n");
        
        //try; //look 
        FILE * reader = fopen(fileName,"rb");
        
        int ID;
        fseek(reader,sizeof(header),SEEK_SET);
        
        if(fread(&ID,sizeof(int),1,reader)==0){
            return 0;
        }
        
        printf("%d",ID);
        
    return 0;
}

void PRINTIN(char *result,int n,char mat[],char nom[],int del){

    
    sprintf(result,"%d*",n);                //n*
    sprintf(result,"%s%d#",result,del);     //n*deleted#
    strncat(result,mat,12);                 //n*deleted#mat
    strncat(result,"#",2);                  //n*deleted#mat#
    strncat(result,nom,40);                 //n*deleted#mat#nom
    strncat(result,"$",2);                  //n*deleted#mat#nom$
    
}

void DISPLAY_HEADER(){
    
    FILE * f = fopen(fileName,"rb");
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        return;
    }
    
    header H;
    fread(&H, sizeof(H), 1, f);
    
    printf("\n==== HEADER ====\n");
    printf("[%d %d %d %d]",H.FIRSTB,H.LASTB,H.FREE_POSITION_B,H.DELETED);
    printf("\n================\n");
}
void addBloc(Bloc *b){
    FILE * writer = fopen(fileName,"ab");
    
    if(writer==NULL){
        printf("Unable to open the file[from addBloc].\n");
        return;
    }
    fwrite(b,sizeof(Bloc),1,writer);//b is a pointer[contain address].
    fclose(writer);
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
        printf("Bloc Loaded Successfuly!\n");
    }
    else{
        printf("Precess Of Loading Bloc Failed!");
    }
    fclose(reader);
    
    return b;
}
void displayBloc(Bloc b){
    
    printf("\ttab: %s\n",b.tab);
    Etudiant* myEtudiants = convertBE(&b);
    displayEtudiants(myEtudiants, b.nb);
    printf("\tnb: %d - suivant: %d\n",b.nb,b.sv);
    
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

void displayEtudiants(const Etudiant* etudiants, int nbEtudiants) {
    for (int i = 0; i < nbEtudiants; i++) {
        printf("\t\tID: %d | Nom: %s | Matricule: %s | Deleted: %d\n", etudiants[i].id, etudiants[i].nom, etudiants[i].mat, etudiants[i].deleted);
    }
}

void AddToBloc(char *s,char str[60]){
    
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

int HEADER(int INDEX){
    
    FILE * f =fopen(fileName,"rb");
    header H;
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        return -111;        //value of null
    }
    fread(&H, sizeof(H), 1, f);    
        
    switch(INDEX){
        case 1:return H.FIRSTB;
            break;
        case 2:return H.LASTB;
            break;
        case 3:return H.FREE_POSITION_B;
            break;
        case 4:return H.DELETED;
            default:
            return -222;    //INDEX OverFlow Value
    }
    fclose(f);
}

void createIndex(int nb){
    // step 0 : verifying if the file exist.
    FILE * indexReaderPtr = fopen(indexFileName,"rb");
    if(indexReaderPtr!=NULL){
        fclose(indexReaderPtr);
        return;
    }
    //step 1 : 
    FILE * indexPointer = fopen(indexFileName,"wb");
    IndexHeader IH;
    IH.nb=nb;

    fwrite(&IH,sizeof(IndexHeader),1,indexPointer);
    fclose(indexPointer);
}
void setIndexHeader(int nb){
  
    FILE * indexPointer = fopen(indexFileName,"rb+");
    IndexHeader IH;
    IH.nb=nb;
    
    fwrite(&IH,sizeof(IndexHeader),1,indexPointer);
    fclose(indexPointer);
}
int getIndexHeader(){
    FILE * indexReaderPtr = fopen(indexFileName,"rb");
    if(indexReaderPtr==NULL){
        return -1;
    }
    IndexHeader IH;
    fread(&IH,sizeof(IndexHeader),1,indexReaderPtr);
    fclose(indexReaderPtr);

    return IH.nb;

}
// adding sort index signature


void addElementToIndex(int cle,int i,int j){
    
    FILE* indexPointer = fopen(indexFileName, "ab");
    if (indexPointer == NULL) {
        printf("Opening File in mode ab Error.");
        return;
    }

    printf("\n\t => Adding Element to Index...\n");

    IndexElement IE;
    IE.cle = cle;
    IE.i = i;
    IE.j = j;

    fwrite(&IE, sizeof(IE), 1, indexPointer);
    fclose(indexPointer);

    printf("\t Added Element: Cle=%d, i=%d, j=%d\n", cle, i, j);
    printf("\t nb in index is : [%d]\n", getIndexHeader() + 1);

    setIndexHeader(getIndexHeader() + 1);

    // After adding the element, re-sort the index file
    sortIndexFile();
}
//Écrire à SFSD_GRP_02
//Complete The Index Function.
void swap(IndexElement *xp, IndexElement *yp) {
    IndexElement temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(IndexElement arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compare based on 'cle' field for sorting
            if (arr[j].cle > arr[j + 1].cle) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}
// Binary search function
int SearchForId(IndexElement array[], int left, int right, int cle) {
    
    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if the target is present at the middle
        if (array[mid].cle == cle)
            return mid;

        // If the target is greater, ignore the left half
        if (array[mid].cle < cle)
            left = mid + 1;
        // If the target is smaller, ignore the right half
        else
            right = mid - 1;
    }

    // If the target is not present in the array
    return -1;

    
}

void sortIndexFile(){
    
    FILE * indexReader = fopen(indexFileName,"rb");
    if(indexReader==NULL){return ;}
    
    
    int nb = getIndexHeader();//
    IndexElement * IndexBuffer = (IndexElement*)malloc(sizeof(IndexElement)*nb);
    
    fseek(indexReader,sizeof(IndexHeader),SEEK_SET);
    fread(IndexBuffer,sizeof(IndexElement),nb,indexReader);

    
    fclose(indexReader);
    
    bubbleSort(IndexBuffer,nb);

    FILE * indexWriter = fopen(indexFileName,"rb+");
    
    fseek(indexWriter,sizeof(IndexHeader),SEEK_SET);
    fwrite(IndexBuffer,sizeof(IndexElement),nb,indexWriter);

    fclose(indexWriter);
}

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