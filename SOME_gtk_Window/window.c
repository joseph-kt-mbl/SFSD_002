#include<gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "../../../../../../msys64/mingw64/include/gtk-3.0/gdk/gdktypes.h"
#define SIZE 200
#define MAT_SIZE 12
#define NAME_SIZE 40

char fileName[]="Blocs.bin";
char indexFileName[]="Index.bin";

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
typedef struct {
    GtkWidget *id_entry;
    GtkWidget *matricule_entry;
    GtkWidget *nom_entry;
    GtkWidget *result_label;
} InsertInfo;

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
void PRINTIN(char *result,int n,char mat[],char nom[],int del){

    
    sprintf(result,"%d*",n);                //n*
    sprintf(result,"%s%d#",result,del);     //n*deleted#
    strncat(result,mat,12);                 //n*deleted#mat
    strncat(result,"#",2);                  //n*deleted#mat#
    strncat(result,nom,40);                 //n*deleted#mat#nom
    strncat(result,"$",2);                  //n*deleted#mat#nom$
    
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
void DISPLAY_HEADER();
void addBloc(Bloc * b);
Bloc readBloc();
void displayBloc(Bloc b);
Etudiant* convertBE(Bloc* bloc);
void displayEtudiants(const Etudiant* etudiants, int nbEtudiants);
SearchInfo Recherche(int c);
int removeEtudiant(int c);
void SET_HEADER(int INDEX,int val);
void insertEtudiant(Etudiant etu);
header readHeader();

//index functions prototype

void createIndex(int nb);
void setIndexHeader(int nb);
void addElementToIndex(int cle, int i, int j);
int getIndexHeader();
IndexElement * readIndex();

//sorting IndexFile.

void sortIndexFile();
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


//searching:
int SearchForId(IndexElement array[], int left, int right, int target);
SearchInfo Search(int cle);

//display :

void displayIndex(){
    int nb = getIndexHeader();
    IndexElement * elements = (IndexElement*)malloc(sizeof(IndexElement)*nb);
    
    FILE * indexReader = fopen(indexFileName,"rb");
    fseek(indexReader,sizeof(IndexHeader),SEEK_SET);
    fread(elements,sizeof(IndexElement),nb,indexReader);
    
    printf("\n\t==================================\n");
    for(int k=0;k<nb;k++){
        printf("\t\tCle : %d ,i: %d,j: %d\n",elements[k].cle,elements[k].i,elements[k].j);
    }
    printf("\t==================================\n");
    fclose(indexReader);
}

static GtkWidget *search_or_delete_result;
static GtkWidget *insertResult;

static void on_rechercher_clicked(GtkButton *button, gpointer data);
static void on_button_clicked(GtkButton *button, gpointer data);
static void on_delete_clicked(GtkButton *button, gpointer data);
static void on_insert_clicked(GtkButton *button, gpointer data);
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *title_label;
    GtkWidget *search_or_delete_label,*ID_label;
    GtkWidget *id_entry;
    GtkWidget *search_button, *delete_button;


    GtkWidget *insert_label;
    GtkWidget *idInsertLabel,*matricule_label,*nom_label; 
    GtkWidget *id_insert_entry, *matricule_entry, *nom_entry;
    GtkWidget *insert_button;

   // GdkColor titleClr = ;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "GTK Example Window");

    // Create a CSS provider
    GtkCssProvider *window_style = gtk_css_provider_new();
    gtk_css_provider_load_from_data(window_style, "window{ font-weight:bold;font-size:16px;}",-1,NULL);

    // Apply the CSS provider to the widget
    GtkStyleContext *window_context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(window_context,
                                   GTK_STYLE_PROVIDER(window_style),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    title_label = gtk_label_new("SFSD PROJECT");
    // Create a CSS provider
    GtkCssProvider *title_style = gtk_css_provider_new();
    gtk_css_provider_load_from_data(title_style, "label { color: #fff;font-weight:bold;font-size:24px;background-color:#f7a311;}",-1,NULL);

    // Apply the CSS provider to the widget
    GtkStyleContext *title_context = gtk_widget_get_style_context(title_label);
    gtk_style_context_add_provider(title_context,
                                   GTK_STYLE_PROVIDER(title_style),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
   
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 2, 1);

    // Search and Delete Section
    search_or_delete_label = gtk_label_new("Rechercher ou Supprimer");
    gtk_grid_attach(GTK_GRID(grid), search_or_delete_label, 0, 1, 2, 1);

    ID_label = gtk_label_new("Entrer id ici : \n[id<999 , id>0]");
    gtk_grid_attach(GTK_GRID(grid),ID_label,0,2,1,1);
    id_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 2, 1, 1);

    search_or_delete_result = gtk_label_new("null");
    gtk_grid_attach(GTK_GRID(grid),search_or_delete_result,0,3,2,1);

    search_button = gtk_button_new_with_label("Rechercher");
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_rechercher_clicked), (gpointer)id_entry);
        GtkCssProvider *search_style = gtk_css_provider_new();
        gtk_css_provider_load_from_data(search_style,
         "button{ color: blue;border:3px solid blue;border-radius:13px }",-1,NULL);

        // Apply the CSS provider to the widget
        GtkStyleContext *search_context = gtk_widget_get_style_context(search_button);
        gtk_style_context_add_provider(search_context,
                                    GTK_STYLE_PROVIDER(search_style),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_grid_attach(GTK_GRID(grid), search_button, 0, 4, 1, 1);


    delete_button = gtk_button_new_with_label("Supprimer");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_clicked), (gpointer)id_entry);
        GtkCssProvider *delete_style = gtk_css_provider_new();
        gtk_css_provider_load_from_data(delete_style
        , "button{ color: red;border:3px solid red;border-radius:13px }",-1,NULL);

        // Apply the CSS provider to the widget
        GtkStyleContext *delete_context = gtk_widget_get_style_context(delete_button);
        gtk_style_context_add_provider(delete_context,
                                    GTK_STYLE_PROVIDER(delete_style),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    gtk_grid_attach(GTK_GRID(grid), delete_button, 1, 4, 1, 1);

    // Insert Section
    insert_label = gtk_label_new("Insertion d'Étudiant");
    gtk_grid_attach(GTK_GRID(grid), insert_label, 0, 5, 2, 1);
    
    idInsertLabel = gtk_label_new("Entrer id ici : \n[id<999 , id>0]");
    gtk_grid_attach(GTK_GRID(grid),idInsertLabel,0,6,1,1);

    id_insert_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), id_insert_entry, 1, 6, 1, 1);

    matricule_label = gtk_label_new("Matricule : ");
    gtk_grid_attach(GTK_GRID(grid),matricule_label,0,7,1,1);

    matricule_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), matricule_entry, 1, 7, 1, 1);

    nom_label = gtk_label_new("Nom : ");
    gtk_grid_attach(GTK_GRID(grid), nom_label, 0, 8, 1, 1);

    nom_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), nom_entry, 1, 8, 1, 1);

    insertResult = gtk_label_new("null");
    gtk_grid_attach(GTK_GRID(grid),insertResult,0,9,2,1);

    InsertInfo *insert_info = g_new(InsertInfo, 1);
    insert_info->id_entry = id_insert_entry;
    insert_info->matricule_entry = matricule_entry;
    insert_info->nom_entry = nom_entry;
    insert_info->result_label = insertResult;

    insert_button = gtk_button_new_with_label("Insérer");
    g_signal_connect(insert_button, "clicked", G_CALLBACK(on_insert_clicked), (gpointer)insert_info);
        GtkCssProvider *insert_style = gtk_css_provider_new();
        gtk_css_provider_load_from_data(insert_style,
         "button{ color: #63de12;border:3px solid #63de12;border-radius:13px }",-1,NULL);

        // Apply the CSS provider to the widget
        GtkStyleContext *insert_context = gtk_widget_get_style_context(insert_button);
        gtk_style_context_add_provider(insert_context,
                                    GTK_STYLE_PROVIDER(insert_style),
                                    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_grid_attach(GTK_GRID(grid), insert_button, 0, 10, 2, 1);


    gtk_widget_show_all(window);
}
bool verifyDigits(const gchar *s){
    for(int i=0;i<strlen(s);i++){
        if(s[i]<'0' || s[i]>'9'){
            return false;
        }
    }
    return true;
}
bool verifyChars(const gchar *s){
    for(int i=0;i<strlen(s);i++){
        if(!((s[i]>='a' && s[i]<='z')||(s[i]>='A' && s[i]<='Z')|| s[i]==' ')){
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    

    CREATE_HEADER(1,0,0,0);  // create File and put header
                            // Confirm That The File Is Created and Header Puted.
    createIndex(0);


    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);   

    return status;
}

static void on_rechercher_clicked(GtkButton *button, gpointer data) {
    GtkWidget *entry_rechercher = GTK_WIDGET(data);
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_rechercher));
    gchar *result_text;
    int cle = atoi(id);

    if(id[0]=='\0'){
        result_text = g_strdup_printf("Veuillez Remplissez Les champs vides");
        gtk_label_set_text(GTK_LABEL(search_or_delete_result),result_text);
        g_free(result_text);
        return;
    }
    if(cle<0){
        result_text = g_strdup_printf("le cle est un eniter positif");
        gtk_label_set_text(GTK_LABEL(search_or_delete_result),result_text);
        g_free(result_text);
        return;
    }


    SearchInfo result = Search(cle);


    if(result.trouv){

        result_text = g_strdup_printf("L'étudiant avec Id:[%d] a été trouvé avec succès\nLe nom est : [%s] ,Matricule : [%s]",cle,(result.etu).nom,(result.etu).mat);
    }
    else{
        result_text = g_strdup_printf("L'etudinat avec Id = %d n'existe pas !",cle);
    }

    gtk_label_set_text(GTK_LABEL(search_or_delete_result), result_text);
    g_free(result_text);

    g_print("Rechercher clicked with id: %s\n", id);
}
static void on_button_clicked(GtkButton *button, gpointer data){
    g_print("Button Clicked\n");
}
static void on_delete_clicked(GtkButton *button, gpointer data){
    GtkWidget *entry_rechercher = GTK_WIDGET(data);
    const gchar *id = gtk_entry_get_text(GTK_ENTRY(entry_rechercher));

   
    gchar *result_text;
    if(id[0]=='\0'){
        result_text = g_strdup_printf("Veuillez Remplissez Les champs vides");
        gtk_label_set_text(GTK_LABEL(search_or_delete_result),result_text);
        g_free(result_text);
        return;
    }
    

    int cle = atoi(id);
    if(cle<0){
        result_text = g_strdup_printf("le cle est un eniter positif");
        gtk_label_set_text(GTK_LABEL(search_or_delete_result),result_text);
        g_free(result_text);
        return;
    }
    int removing_result = removeEtudiant(cle);

    if(removing_result==1){
        result_text = g_strdup_printf("L'etudiant qui a le Cle[%d] a ete bien supprimer !",cle);    
    }
    else{
        result_text = g_strdup_printf("L'etudinat avec Id = %d n'existe pas, nous ne pouvons pas le supprimer.",cle);
    }
    gtk_label_set_text(GTK_LABEL(search_or_delete_result), result_text);
    g_free(result_text);
}
static void on_insert_clicked(GtkButton *button, gpointer data){
    InsertInfo *insert_info = (InsertInfo *)data;

    if (insert_info == NULL) {
        g_print("Error: InsertInfo is NULL\n");
        return;
    }

    gchar *result_text;

   
    const gchar *id_text = gtk_entry_get_text(GTK_ENTRY(insert_info->id_entry));
    
    if(id_text[0]=='\0'){
        result_text = g_strdup_printf("Veuillez Remplissez Les champs vides");
        gtk_label_set_text(GTK_LABEL(insertResult),result_text);
        g_free(result_text);
        return;
    }
    if(!verifyDigits(id_text)){
        result_text = g_strdup_printf("le cle est un entier,ne contient que des chiffres[max 3]");
        gtk_label_set_text(GTK_LABEL(insertResult),result_text);
        g_free(result_text);
        return;
    }
    
    int id = atoi(id_text);
    
    if(id<0){
        result_text = g_strdup_printf("le cle est un eniter positif");
        gtk_label_set_text(GTK_LABEL(insertResult),result_text);
        g_free(result_text);
        return;
    }

    
    
    

    if(Recherche(id).trouv){
        result_text = g_strdup_printf("le clé:[%d] est déjà utilisée \nréessayez avec une autre clé.",id);
        gtk_label_set_text(GTK_LABEL(insertResult),result_text);
        g_free(result_text);
        return;
    }
        const gchar *matricule_text = gtk_entry_get_text(GTK_ENTRY(insert_info->matricule_entry));
        const gchar *nom_text = gtk_entry_get_text(GTK_ENTRY(insert_info->nom_entry));
        if(matricule_text[0]=='\0' ||  nom_text[0]=='\0'){
            
            result_text = g_strdup_printf("Veuillez Remplissez Les champs vides");
            gtk_label_set_text(GTK_LABEL(insertResult),result_text);
            g_free(result_text);
            return;
        }
        if(!(verifyChars(nom_text) && verifyDigits(matricule_text))){
            result_text = g_strdup_printf("Le nom ne contient que des caracters[max 40] ,\nLe matricule ne contient que des chiffres[maximum 12]");
            gtk_label_set_text(GTK_LABEL(insertResult),result_text);
            g_free(result_text);
            return;
        }
    
    Etudiant etu;
    strcpy(etu.mat,matricule_text);
    etu.id=id;
    etu.deleted=0;
    strcpy(etu.nom,nom_text);

    insertEtudiant(etu);

    result_text = g_strdup_printf("L'etudiant a été inséré avec succès");
    gtk_label_set_text(GTK_LABEL(insertResult),result_text);
    g_free(result_text);

    DISPLAY_HEADER();

    
}


void DISPLAY_HEADER(){
    header H = readHeader();
    
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
        
        sscanf(token, "%d*%d#%[^#]#%[^$]",
                &etudiants[count].id, &etudiants[count].deleted,
                etudiants[count].mat, etudiants[count].nom);

        char str[3];// max-len 3
        int i=0;
        
        while(token[i]!='*'){
            str[i]=token[i];
            i++;
        }
        
        char idStr[i];
        int j=0;
        while(j<i){
            idStr[j]=str[j];
            j++;
        }
        
        etudiants[count].id = atoi(idStr);

        token = strtok(NULL, "$");// recuperer : deuxiemme etudiant .

        count++;
    }

    return etudiants;
}
void displayEtudiants(const Etudiant* etudiants, int nbEtudiants) {
    for (int i = 0; i < nbEtudiants; i++) {
        printf("\t\tID: %d | Nom: %s | Matricule: %s | Deleted: %d\n", etudiants[i].id, etudiants[i].nom, etudiants[i].mat, etudiants[i].deleted);
    }
}
SearchInfo Recherche(int c){
    
    FILE * reader = fopen(fileName,"rb");
    SearchInfo result;
    
    result.trouv=false;
    
    if(reader==NULL){
        printf("The File Does not Exist [From Recherche]");
        return result;
    }
    bool trouv=false;
    int i=1;
    
    int fin = HEADER(2);
    
    while(i<=fin && !trouv){
        
        Bloc Buf = readBloc(i);
        
        Etudiant* etudiants = convertBE(&Buf);
        
        for(int j=0;j<Buf.nb;j++){
            if(etudiants[j].id==c){
                fclose(reader);
                result.trouv = true;
                etudiants[j].mat[12]='\0';
                result.etu = etudiants[j];
                result.i=i;
                result.j=j;
                
                return result;
                
            }
        }
        i++;  
        
    }
    fclose(reader);
    return result;

}

int removeEtudiant(int c){
    SearchInfo result = Search(c);

    // Debug print
    printf("Search result for ID %d: trouv=%d, i=%d, j=%d\n", c, result.trouv, result.i, result.j);

    if (!result.trouv) {
        // Debug print
        printf("Etudiant not found for removal\n");
        return 0;
    }

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
    printf("Before removal: BUFFER.tab[a+1] = %c\n", BUFFER.tab[a + 1]);

    // Modify the deletion flag
    BUFFER.tab[a + 1] = '1';

    // Debug print
    printf("After removal: BUFFER.tab[a+1] = %c\n", BUFFER.tab[a + 1]);

    FILE *writer = fopen(fileName, "rb+");
    fseek(writer, sizeof(header) + (result.i - 1) * sizeof(Bloc), SEEK_SET);
    fwrite(&BUFFER, sizeof(Bloc), 1, writer);
    fclose(writer);

    // Debug print
    printf("Etudiant removed successfully\n");

    return 1;
}
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
void insertEtudiant(Etudiant etu){
    
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

//Index Function definition :
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

IndexElement * readIndex(){
    int nb = getIndexHeader();
    IndexElement * elements = (IndexElement*)malloc(sizeof(IndexElement)*nb);
    
    FILE * indexReader = fopen(indexFileName,"rb");
    fseek(indexReader,sizeof(IndexHeader),SEEK_SET);
    fread(elements,sizeof(IndexElement),nb,indexReader);
    
    return elements;
    
}

SearchInfo Search(int cle){
     IndexElement * IEpTr = readIndex();
    int ind = SearchForId(IEpTr,0,getIndexHeader()-1,cle);
    
    if(ind!=-1){
        IndexElement IE = IEpTr[ind];
        Bloc bloc = readBloc(IE.i);

        char* token = strtok(bloc.tab, "$");//recuperer premeier Etudiant.
        
        int count = 0;
        Etudiant etu;

        while (count<IE.j) {
            printf("\n\t\ttoken[%d] => %s \n",count,token);
            token = strtok(NULL, "$");// recuperer : deuxiemme etudiant .
            count++;
        }

        etu.mat[12]='\0';

        sscanf(token, "%d*%d#%[^#]#%[^$]",
                    &etu.id, &etu.deleted,etu.mat,etu.nom);


            char str[3];// max-len 3[023]
            int i=0;
            
            while(token[i]!='*'){
                str[i]=token[i];
                i++;
            }
            
            char idStr[i];//2
            int j=0;
            while(j<i){
                idStr[j]=str[j];
                j++;
            }
            
            etu.id = atoi(idStr);

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

