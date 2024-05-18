#ifndef UNTITLED15_STRUCT_TABLE_H
#define UNTITLED15_STRUCT_TABLE_H

enum Error{
    MEMORY_ERROR,
    INPUT_ERROR,
};

//Тип ключей
typedef struct KeyType{
    int data;
}KeyType;
//-------------------------------


//тип
typedef struct InfoType{
    char* data;
}InfoType;
//-------------------------------

//Тип
typedef struct IndexType{
    int* data;
}IndexType;
//-------------------------------


//Пространство ключей
typedef struct KeySpace {
    KeyType key;
    KeyType par;
    InfoType *info;
    struct KeySpace *next;
} KeySpace;
//-------------------------------


//Таблица
typedef struct Table {
    KeySpace *ks;
} Table;
//-------------------------------

//Функции
KeySpace *createKeySpace(KeyType key, KeyType par, char *data);
int insertIntoTable(Table *table, KeyType key, char *data, KeyType parentKey);
int deleteFromTable(Table *table, KeyType key);
KeySpace *findInTable(Table *table, KeyType key);
void printTable(Table *table);
int recursiveDeleteFromTable(Table *table, KeyType key);
Table *findAllByParentKey(Table *table, KeyType parentKey);
void freeTable(Table *table);
int importDataFromFile(Table *table, const char *filename);



#endif