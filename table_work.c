#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "struct_table.h"

KeySpace *createKeySpace(KeyType key, KeyType par, char *data) {
    KeySpace *newKeySpace = (KeySpace *)malloc(sizeof(KeySpace));
    if (newKeySpace == NULL) {
        return NULL;
    }
    newKeySpace->key = key;
    newKeySpace->par = par;
    newKeySpace->info = (InfoType *)malloc(sizeof(InfoType));
    if (newKeySpace->info == NULL) {
        free(newKeySpace);
        return NULL;
    }
    newKeySpace->info->data = strdup(data);
    if (newKeySpace->info->data == NULL) {
        free(newKeySpace->info);
        free(newKeySpace);
        return NULL;
    }
    newKeySpace->next = NULL;
    return newKeySpace;
}


int insertIntoTable(Table *table, KeyType key, char *data, KeyType parentKey) {
    KeySpace *newKeySpace = createKeySpace(key, parentKey, data);
    if (newKeySpace == NULL) {
        return MEMORY_ERROR;
    }

    newKeySpace->next = table->ks;
    table->ks = newKeySpace;

    return 0;
}

int deleteFromTable(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    KeySpace *prev = NULL;
    while (current != NULL) {
        if (current->key.data == key.data) {
            if (prev == NULL) {
                table->ks = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->info->data);
            free(current->info);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return INPUT_ERROR;
}


KeySpace *findInTable(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    KeySpace *result = NULL;
    while (current != NULL) {
        if (current->key.data == key.data) {
            KeySpace *newKeySpace = createKeySpace(current->key, current->par, current->info->data);
            if (newKeySpace == NULL) {
                return NULL;
            }
            newKeySpace->next = result;
            result = newKeySpace;
        }
        current = current->next;
    }
    return result;
}

void printTable(Table *table) {
    KeySpace *current = table->ks;
    while (current != NULL) {
        printf("Key: %d, Parent Key: %d, Data: %s\n", current->key.data, current->par.data, current->info->data);
        current = current->next;
    }
}


// Функция для удаления всех элементов, ссылающихся на удаляемый
void deleteAllReferences(KeySpace *current, Table *table, KeyType key);

// Функция для удаления элемента из таблицы по заданному значению ключа и всех элементов, ссылающихся на удаляемый
int recursiveDeleteFromTable(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    KeySpace *prev = NULL;
    while (current != NULL) {
        if (current->key.data == key.data) {
            deleteAllReferences(table->ks, table, key);
            deleteFromTable(table, key);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return INPUT_ERROR;
}

// Функция для удаления всех элементов, ссылающихся на удаляемый
void deleteAllReferences(KeySpace *current, Table *table, KeyType key) {
    if (current == NULL) {
        return;
    }
    deleteAllReferences(current->next, table, key);
    if (current->par.data == key.data) {
        recursiveDeleteFromTable(table, current->key);
    }
}

Table *findAllByParentKey(Table *table, KeyType parentKey) {
    Table *resultTable = (Table *)malloc(sizeof(Table));
    if (resultTable == NULL) {
        return NULL;
    }
    resultTable->ks = NULL;

    KeySpace *current = table->ks;
    KeySpace *prevResult = NULL;

    while (current != NULL) {
        if (current->par.data == parentKey.data) {
            KeySpace *newKeySpace = createKeySpace(current->key, current->par, current->info->data);
            if (newKeySpace == NULL) {
                freeTable(resultTable);
                return NULL;
            }
            if (resultTable->ks == NULL) {
                resultTable->ks = newKeySpace;
            } else {
                prevResult->next = newKeySpace;
            }
            prevResult = newKeySpace;
        }
        current = current->next;
    }

    return resultTable;
}



void freeTable(Table *table) {
    KeySpace *current = table->ks;
    KeySpace *next;
    while (current != NULL) {
        next = current->next;
        free(current->info->data);
        free(current->info);
        free(current);
        current = next;
    }
    free(table);
}

int importDataFromFile(Table *table, const char *filename) {
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return INPUT_ERROR;
    }

    char line[256];
    rewind(file);

    while (fgets(line, sizeof(line), file) != NULL) {
        int key, parentKey;
        char data[100];
        if (sscanf(line, "%d %d %99[^\n]", &key, &parentKey, data) != 3) {
            printf("Ошибка при чтении строки из файла.\n");
            fclose(file);
            return INPUT_ERROR;
        }
        insertIntoTable(table, (KeyType){key}, data, (KeyType){parentKey});
    }

    fclose(file);
    return 0;
}





