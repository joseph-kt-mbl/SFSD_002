// index.h

#ifndef INDEX_H
#define INDEX_H

typedef struct {
    int nb;
} IndexHeader;

typedef struct {
    int cle;
    int i;
    int j;
} IndexElement;

// Function Declarations
void createIndex(int nb);
IndexElement *readIndex();
void setIndexHeader(int nb);
int getIndexHeader();

void addElementToIndex(int cle, int i, int j);

int SearchForId(IndexElement array[], int left, int right, int cle);
void swap(IndexElement *xp, IndexElement *yp);
void bubbleSort(IndexElement arr[], int n);
void sortIndexFile();


#endif // INDEX_H
