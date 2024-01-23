#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
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

//read header.
header readHeader();
int HEADER(int INDEX);
//adding Set Header
void SET_HEADER(int INDEX, int val);
void addBloc(Bloc * b);
void AddToBloc(char *s,char str[60]);
Bloc readBloc();

Etudiant* convertBE(Bloc* bloc);

void createIndex(int nb);
//return array of IndexElement
IndexElement *readIndex();
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
void removeEtudiant(int c);
//insertion.
void insertEtudiant(Etudiant etu);
//Used Id
bool UsedId(int cle);

//display
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


int main(){   
    char *nom1="Abdellaoui Youcef";
    char *nom2="Ghaoui Wail";
    char *nom3="Naili Walid";
    char *mat1="222231609707";
    char *mat2="222231609707";
    char *mat3="222231609707";
    
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
void PRINTIN(char *result,int n,char mat[],char nom[],int del){

    
    sprintf(result,"%d*",n);                //n*
    sprintf(result,"%s%d#",result,del);     //n*deleted#
    strncat(result,mat,12);                 //n*deleted#mat
    strncat(result,"#",2);                  //n*deleted#mat#
    strncat(result,nom,40);                 //n*deleted#mat#nom
    strncat(result,"$",2);                  //n*deleted#mat#nom$
    
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

//return array of IndexElement
IndexElement * readIndex(){
    int nb = getIndexHeader();
    IndexElement * elements = (IndexElement*)malloc(sizeof(IndexElement)*nb);
    
    FILE * indexReader = fopen(indexFileName,"rb");
    fseek(indexReader,sizeof(IndexHeader),SEEK_SET);
    fread(elements,sizeof(IndexElement),nb,indexReader);
    
    return elements;
    
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
//Confirm That the Id=cle is unic.
bool UsedId(int cle){
    IndexElement * IEpTr = readIndex();
    int ind = SearchForId(IEpTr,0,getIndexHeader()-1,cle);
    
    return ind==-1?false:true;
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

//display
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