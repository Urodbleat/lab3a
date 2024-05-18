#include <stdio.h>
#include <stdlib.h>
#include "struct_table.h"
#include "menu.h"

int main() {
    Table *mainTable = (Table *)malloc(sizeof(Table));
    if (mainTable == NULL) {
        return MEMORY_ERROR;
    }
    mainTable->ks = NULL;

    menu(mainTable);

    return 0;
}
