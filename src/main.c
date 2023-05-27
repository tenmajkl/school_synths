/**
 * Synths - main
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#include<stddef.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"
#include "actions.h"
#include "helpers.h"
#include "lists.h"

/**
 * Menu items
 */
const int menu_items_count = 7;
const action_t menu_items[] = {
    {"Vypsat data", print},
    {"Seradit", sortAction},
    {"Filtrovat", filterDialogueAction},
    {"Najit nejstarsi syntezator", oldestAction},
    {"Pridat syntezator", addItemAction},
    {"Smazat syntezator", deleteAction},
    {"Upravit syntezator", editAction},
};

/**
 * Prints menu to stdout
 */
void printMenu(color_t color)
{
    printf("0: Ukoncit\n");
    for (int index = 0; index < menu_items_count; index++) {
        printfc(color, "%i: %s\n", index+1, menu_items[index].description);
    }
}

/**
 * Menu interface
 */
void menu(synthesizer_array_t* list)
{
    int choice = -1;
    do {
        clear();
        printMenu(YELLOW);
        printf("Zadej moznost: ");
        int scan_result = scanf("%i", &choice);
        
        if (scan_result != 1 || choice < 0 || choice > menu_items_count) {
            error(4);
            waitForClick();
            continue;
        }

        if (choice == 0) {
            return;
        }

        int code;
        if ((code = menu_items[choice - 1].function(list)) != 0) {
            error(code);
        }
        waitForClick();
    } while (choice); // condition is redundant, but still...
}


int main(void)
{
    char file_name[32];
    printf("Zadej jmeno souboru: ");
    scanf("%31s", file_name);

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return -1;
    }
    synthesizer_array_result_t loaded = load(file);

    if (loaded.error != 0) {
        error(loaded.error);
        waitForClick();
    }

    synthesizer_array_t list = loaded.result;

    fclose(file);

    menu(&list);

    char save;
    clearBuffer();
    printf("Ulozit zmeny? y/n: ");
    scanf("%c", &save);

    while (save != 'n') {
        printf("Zadej jmeno souboru: ");
        scanf("%31s", file_name);
        FILE* file = fopen(file_name, "w");
        if (file == NULL) {
            free(list.array);
            return -1;
        }

        write(file, list, DATA_FORMAT);

        fclose(file);
        clearBuffer();
        printf("Ulozit zmeny do dalsiho souboru? y/n: ");
        scanf("%c", &save);
    }

    free(list.array);
    return 0;
}
