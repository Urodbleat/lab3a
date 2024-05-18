#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_table.h"

void send_command_menu() {
    printf("\033[0;33m____________________________________\n");
    printf("__________Вот ваше меню__________\n");
    printf("____Выберите одну из опций____\n");
    printf("[1] Добавить новый элемент\n");
    printf("[2] Удалить элемент\n");
    printf("[3] Поиск по ключу\n");
    printf("[4] Вывести таблицу\n");
    printf("[5] Удалить элемент и все элементы, ссылающиеся на него\n");
    printf("[6] Поиск всех элементов по ключу родительского элемента\n");
    printf("[7] Загрузить данные из файла\n");
    printf("[8] Выход\n");
    printf("____________________________________\033[0m\n");
}

void menu(Table *mainTable) {
    printf("\033[H\033[J");
    send_command_menu();
    while (1) {
        printf("\033[1;90mВведите команду (число [1-8]):\033[0;0m\n");
        int command;
        if (scanf("%d", &command) != 1) {
            printf("Некорректный ввод, попробуйте еще раз\n");
            scanf("%*[^\n]*c");
            continue;
        }
        switch (command) {
            case 1:
                printf("Введите данные нового элемента: ");
                char data[100];
                scanf("%s", data);

                printf("Введите ключ элемента: ");
                int key;
                if (scanf("%d", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }

                printf("Введите ключ родительского элемента: ");
                int parentKey;
                if (scanf("%d", &parentKey) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }

                insertIntoTable(mainTable, (KeyType){key}, data, (KeyType){parentKey});
                break;
            case 2:
                printf("Введите ключ элемента для удаления: ");
                if (scanf("%d", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                deleteFromTable(mainTable, (KeyType){key});
                break;

            case 3:
                printf("Введите ключ для поиска: ");
                if (scanf("%d", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                KeySpace *result = findInTable(mainTable, (KeyType){key});
                if (result != NULL) {
                    printf("Найденный элемент:\n");
                    printf("Ключ: %d, Родительский ключ: %d, Данные: %s\n", result->key.data, result->par.data, result->info->data);
                    free(result->info->data);
                    free(result->info);
                    free(result);
                } else {
                    printf("Элемент с ключом %d не найден\n", key);
                }
                break;

            case 4:
                printf("Содержимое таблицы:\n");
                printTable(mainTable);
                break;

            case 5:
                printf("Введите ключ элемента для удаления: ");
                if (scanf("%d", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                recursiveDeleteFromTable(mainTable, (KeyType){key});
                break;

            case 6:
                printf("Введите ключ родительского элемента для поиска: ");
                if (scanf("%d", &key) != 1) {
                    printf("Некорректный ввод, попробуйте еще раз\n");
                    scanf("%*[^\n]*c");
                    continue;
                }
                Table *parentKeyTable = findAllByParentKey(mainTable, (KeyType){key});
                printf("Результаты поиска по ключу родительского элемента:\n");
                printTable(parentKeyTable);
                freeTable(parentKeyTable);
                break;
            case 7:
                importDataFromFile(mainTable, "data.txt");
                break;

            case 8:
                freeTable(mainTable);
                return;
            default:
                printf("Неверная команда, пожалуйста, попробуйте еще раз\n");
                break;
        }
    }
}