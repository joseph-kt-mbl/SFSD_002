#include<stdio.h>
#include"primitives.h"
#include"index.h"
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

char indexFileName[]="Index.bin";

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
IndexElement * readIndex(){
    int nb = getIndexHeader();
    IndexElement * elements = (IndexElement*)malloc(sizeof(IndexElement)*nb);
    
    FILE * indexReader = fopen(indexFileName,"rb");
    fseek(indexReader,sizeof(IndexHeader),SEEK_SET);
    fread(elements,sizeof(IndexElement),nb,indexReader);
    
    return elements;
    
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

    printf("\n\t\t\t => Adding Element to Index...\n");

    IndexElement IE;
    IE.cle = cle;
    IE.i = i;
    IE.j = j;

    fwrite(&IE, sizeof(IE), 1, indexPointer);
    fclose(indexPointer);

    printf("\t\t\t    Added Element: Cle=%d, i=%d, j=%d\n", cle, i, j);
    printf("\t\t\t    nb in index is : [%d]\n", getIndexHeader() + 1);

    setIndexHeader(getIndexHeader() + 1);

    // After adding the element, re-sort the index file
    sortIndexFile();
}
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