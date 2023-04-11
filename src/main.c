/**
 * Synths
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3
 * 
 * Notes:
 *  - shoegaze is based
 *  - my relationship with c is love-hate
 *
 * TODO:
 *  - split it into files
 */

#include<stddef.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "types.h"

#define DATA_FORMAT "%d|%s|%s|%d|%d|%d\n"
#define PRETTY_FORMAT "| %2d | %15s | %15s | %10d | %16d | %9d |\n"

/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

const char errors[][64] = {
    "Nelze pridat prvek, v pameti je malo mista.",
    "Pole je prazdne",
    "Nepodarilo se vytvorit pomocne pole.",
    "Spatna volba",
    "Doslo misto v pameti!",
    "Chyba formatu",
    "Nepodarilo se najit prvek v poli",
    "Neznama polozka",
};

/**
 * Outputs error message of given code
 *
 * @param code Code of error from errors array
 */
void error(int code)
{
    printf("[ERROR] %s\n", errors[code - 1]);
}

// --- Helpers ---

/**
 * Clears screen depending on curent OS
 */
void clear(void)
{
    #if defined __WIN32 || defined __WIN64
        system("cls"); // Windows has different command for clearing.
    #else
        system("clear"); // If the os is not windows its propably unix-like.
    #endif
}

/**
 * Clears buffer of stdin which is used when reading input
 */
void clearBuffer(void)
{
    while ((getchar()) != '\n');
}

/**
 * Waits for user to click before new actions
 */
void waitForClick(void)
{
    clearBuffer();
    puts("Zmackni enter pro pokracovani");
    getchar();
}

// --- Lists and structures ---

/**
 * Loads data from file to array
 *
 * Can return codes 5, 6, 0
 */
synthesizer_array_result_t load(FILE* input)
{
    synthesizer_array_result_t result;
    result.error = 0; 
    synthesizer_array_t array;
    array.array = malloc(16 * sizeof(synthesizer_t));
    array.size = 0;
    array.capacity = 16;
    synthesizer_t* new;
    int analog;
    int loading_result;

    while ((loading_result = fscanf(input, "%d|%15[^|\n]|%15[^|\n]|%d|%d|%d\n",
                &array.array[array.size].id,
                array.array[array.size].name,
                array.array[array.size].manufacturer,
                &array.array[array.size].year,
                &array.array[array.size].voices,
                &analog
            )) == 6
    ) {
        array.array[array.size].analog = analog != 0;
        array.array[array.size].deleted = false;
        array.size++;
        if (array.size == array.capacity) {
            array.capacity += 16;
            new = realloc(array.array, array.capacity * sizeof(synthesizer_t));
            if (new == NULL) {
                result.result = array;
                result.error = 5;
                return result;
            }
            array.array = new;
        }
    }

    if (loading_result != EOF) {
        result.result = array;
        result.error = 6;
        return result;
    }

    result.result = array;
    return result;
}

/**
 * Copies one list to another
 */
synthesizer_array_t copy(synthesizer_array_t list)
{
    synthesizer_array_t result = list;
    size_t size = list.size * sizeof(synthesizer_t);
    synthesizer_t* array = malloc(size);

    memcpy(array, list.array, size);

    result.array = array;
    return result;
}

// --- Output ---

/**
 * Outputs one item to dedicated file with given format string
 */
void writeOne(FILE* output, char* format, synthesizer_t item)
{
    fprintf(output, format, item.id, item.name, item.manufacturer, item.year, item.voices, item.analog ? 1 : 0);
}

/** 
 * Outputs list to dedicated file with given format string
 */
void write(FILE* output, synthesizer_array_t array, char* format)
{
    synthesizer_t item;
    for (int index = 0; index < array.size; index++) {
        item = array.array[index];
        if (item.deleted) {
            continue;
        }

        writeOne(output, format, item);
    }
}

/**
 * Outputs head of table
 */
void printHead()
{
    puts(
        "+----+-----------------+-----------------+------------+------------------+-----------+\n"
        "| id | Model           | Vyrobce         | Rok vydani | Pocet oscilatoru | Analogovy |\n"
        "|----+-----------------+-----------------+------------+------------------+-----------|"
    );
}

/**
 * Prints whole list to stdout with pretty format
 */
int print(synthesizer_array_t* list)
{
    printHead();
    write(stdout, *list, PRETTY_FORMAT);
    return 0;
}

// --- Filtering ---

synthesizer_array_result_t filter(synthesizer_array_t list, synthesizer_t key, condition_t condition)
{
    synthesizer_array_result_t result;
    result.error = 0;
    synthesizer_array_t array;
    array.capacity = list.capacity;
    array.size = 0;
    array.array = malloc(list.size * sizeof(synthesizer_t));

    if (array.array == NULL) {
        result.error = 2;
        result.result = list;
        return result;
    }

    for (int index = 0; index < list.size; index++) {
        if (!list.array[index].deleted && condition(list.array[index], key) == 0) {
            array.array[array.size] = list.array[index];
            array.size++;
        }
    }

    if (array.size < list.size) {
        synthesizer_t* new = realloc(array.array, array.size * sizeof(synthesizer_t));
        if (new == NULL) {
            result.error = 3;
            result.result = array;
            return result;
        }
        array.array = new;
        array.capacity = array.size;
    }

    result.result = array;
    return result;
}

/**
 * Returns oldest synth
 */
synthesizer_result_t getOldest(synthesizer_array_t list)
{
    if (list.size == 0) {
        return (synthesizer_result_t) { .error = 2 };
    }
    synthesizer_t* oldest = &(list.array[0]);
    for (int index = 1; index < list.size; index++) {
        if (list.array[index].year < oldest->year) {
            oldest = &(list.array[index]);
        }
    }
    return (synthesizer_result_t) { oldest, 0 };
}

// --- Comparing functions ---

/**
 * Compares two items by id
 */
int byIdCondition(synthesizer_t first, synthesizer_t second)
{
    return first.id - second.id;
}

/**
 * Compares two items by year
 */
int byYearCondition(synthesizer_t first, synthesizer_t second)
{
    return first.year - second.year;
}

/**
 * Compares two items by manufacturer
 */
int byManufacturerCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.manufacturer, second.manufacturer);
}

/**
 * Compares two items by name
 */
int byNameCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.name, second.name);
}

/**
 * Compares two items by number of voices
 */
int byVoicesCondition(synthesizer_t first, synthesizer_t second)
{
    return first.voices - second.voices;
}

/**
 * Compares two items by analogness
 */
int byAnalogCondition(synthesizer_t first, synthesizer_t second)
{
    return first.analog - second.analog;
}

// --- Fields ---

/**
 * Fields which can be used for editing or grouping
 */
const int field_count = 5;
const synthesizer_field_t fields[] = {
    { NAME, "Podle jmena", "Jmeno", "%15s", byNameCondition },
    { MANUFACTURER, "Podle vyrobce", "Vyrobce", "%15s", byManufacturerCondition },
    { YEAR, "Podle roku vydani", "Rok vydani", "%d", byYearCondition },
    { VOICES, "Podle poctu hlasu", "Pocet hlasu", "%d", byVoicesCondition },
    { ANALOG, "Podle analogovosti", "Analogovy", "%d", byAnalogCondition }
};

/**
 * Outputs menu for filters
 */
void fieldsFilterMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].filter_description);
    }
}

/**
 * Outputs menu for editing
 */
void fieldsEditMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].edit_description);
    }
}

/**
 * Loads given value to given key from user 
 *
 * This allows us to dynamicaly edit and create keys for filtering
 */
void getKeyByField(synthesizer_array_t list, synthesizer_field_t field, synthesizer_t* key)
{
    printf("Zadej %s: ", field.edit_description);
    clearBuffer();
    char analog;
    
    switch (field.index) {
        case NAME:
            scanf("%15s", key->name);
            break;
        case MANUFACTURER:
            scanf("%15s", key->manufacturer);
            break;
        case YEAR:
            scanf("%d", &key->year);
            break;
        case VOICES:
            scanf("%d", &key->voices);
            break;
        case ANALOG:
            scanf("%c", &analog);
            key->analog = analog == 'y';
            break;
    }

}

/**
 * Loads field from input
 */
synthesizer_field_tResult getField()
{
    int index;
    printf("Zadej cislo polozky: ");
    scanf("%d", &index);

    if (index < 0 || index >= field_count) {
        return (synthesizer_field_tResult) { .error = 8 };
    }

    return (synthesizer_field_tResult) { fields[index], 0 };
}

// --- Sorting ---

/**
 * Merge sort melting
 */
void melt(synthesizer_array_t* list, synthesizer_array_t* help, int from, int to, int middle, condition_t compare)
{
    int left = from, right = middle + 1, index = from;
    while (left <= middle && right <= to) {
        if (compare(list->array[left], list->array[right]) <= 0) {
            help->array[index++] = list->array[left++];
        } else {
            help->array[index++] = list->array[right++];
        }
    }

    while (left <= middle) {
        help->array[index++] = list->array[left++];
    }

    while (right <= to) {
        help->array[index++] = list->array[right++];
    }
}

/**
 * Merge sort algorithm
 */
void mergeSort(synthesizer_array_t* list, synthesizer_array_t* help, int from, int to, condition_t condition)
{
    if (from >= to) {
        return;
    }

    int middle = (from + to) / 2;
    mergeSort(list, help, from, middle, condition);
    mergeSort(list, help, middle + 1, to, condition);

    melt(list, help, from, to, middle, condition);

    while (from <= to) {
        list->array[from] = help->array[from];
        from++;
    }
}

/**
 * Sorts data using merge sort
 *
 * Basicaly helper for sorting which makes it simpler to work with
 */
int sort(synthesizer_array_t* list, condition_t condition)
{
    synthesizer_array_t help = *list;
    help.array = malloc(help.size * sizeof(synthesizer_t));
    if (help.array == NULL) {
        return 3;
    }
    mergeSort(list, &help, 0, list->size - 1, condition);
    free(help.array);
    return 0;
}
/**
 * Generic user interface for sorting
 */
int sortDialogue(synthesizer_array_t* list, condition_t condition)
{
    int choice;
    printf("Chces data jen vypsat (1), nebo i ulozit (2)? Pozn. pouze vypsani zabere vice pameti. ");
    scanf("%i", &choice);
    if (choice == 1) {
        synthesizer_array_t copied = copy(*list);
        int code;
        if ((code = sort(&copied, condition)) != 0) {
            return code;
        }
        printHead();
        write(stdout, copied, PRETTY_FORMAT);
        free(copied.array);
        return 0;
    }

    return sort(list, condition);
}

// --- Searching ---

/**
 * Searches for item with given comparing function and key.
 *
 * Since we're not sure that the array is sorted by name,
 * binary search is not acceptable since with sorting it would be snailing slow.
 *
 * Other way of solving this would be using filter with byNameCondition and then take first element
 * However this returns array of items, not pointers, so it won't be editable 
 * and it would be even less effective than this.
 */
synthesizer_result_t search(synthesizer_array_t list, synthesizer_t key, condition_t compare)
{
    if (list.size == 0) {
        return (synthesizer_result_t) { .error = 2 };
    }

    int index = 0;

    while (index < list.size 
            && (
                compare(list.array[index], key) != 0
                || list.array[index].deleted
            )
    ) {
        index++;
    }

    return index == list.size
        ? ((synthesizer_result_t) { .error = 7})
        : ((synthesizer_result_t) { &list.array[index], 0})
    ;
}

/**
 * Dialogue over searching
 */
synthesizer_result_t searchWithDialogue(synthesizer_array_t *list)
{
    synthesizer_t key;
    printf("Zadej id: ");
    scanf("%d", &key.id);

    synthesizer_result_t result = search(*list, key, byIdCondition);

    if (result.error != 0) {
        return (synthesizer_result_t) { .error = result.error };
    }

    return (synthesizer_result_t) { result.result, 0 };
}

// --- Actions from menu ---

/**
 * Adds item to list
 */
int addItemAction(synthesizer_array_t* list)
{
    synthesizer_t* new;
    if (list->size == list->capacity) {
        list->capacity++;
        new = realloc(list->array, list->size);
        if (new == NULL) {
            return 1;
        }
        list->array = new;
    }
    list->size++;

    synthesizer_t item;

    printf("Zadej jmeno modelu: ");
    scanf("%15s", item.name);

    printf("Zadej jmeno vyrobce: ");
    scanf("%15s", item.manufacturer);

    printf("Zadej rok vydani: ");
    scanf("%d", &item.year);

    printf("Zadej pocet hlasu: ");
    scanf("%d", &item.voices);

    clearBuffer();
    char analog;
    printf("Je analogovy? y/n: ");
    scanf("%c", &analog);
    item.analog = analog == 'y';

    item.deleted = false;

    item.id = list->size > 1 ? list->array[list->size - 2].id + 1 : 1;

    list->array[list->size - 1] = item;
    return 0;
}

/**
 * Prints models from loaded manufacturer
 */
int filterDialogueAction(synthesizer_array_t* list)
{ 
    fieldsFilterMenu();
    synthesizer_field_tResult field = getField();
    if (field.error != 0) {
        return field.error;
    }
    synthesizer_t key;
    getKeyByField(*list, field.result, &key);
    printHead();

    synthesizer_array_result_t models = filter(*list, key, field.result.condition);
    if (models.error != 0) {
        return models.error;
    }
    
    write(stdout, models.result, PRETTY_FORMAT);

    free(models.result.array);

    return 0;
}

/**
 * Menu item for sorting by year
 */
int sortByYearAction(synthesizer_array_t* list)
{
    return sortDialogue(list, byYearCondition);
}

/**
 * Menu item for sorting by name
 */
int sortByNameAction(synthesizer_array_t* list)
{
    return sortDialogue(list, byNameCondition);
}

/**
 * Searches for given item
 */
int searchDialogue(synthesizer_array_t *list)
{
    synthesizer_result_t result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *result.result);
    return 0;
}

/**
 * Edits given item
 */
int editAction(synthesizer_array_t* list)
{
    synthesizer_result_t result = searchWithDialogue(list);

    if (result.error != 0) {
        return result.error;
    }

    synthesizer_t* item = result.result;

    fieldsEditMenu();
    synthesizer_field_tResult field = getField(); 
    if (field.error != 0) {
        return field.error;
    }
    getKeyByField(*list, field.result, item);

    return 0;
}

/**
 * Removes given item
 */
int deleteAction(synthesizer_array_t *list)
{
    synthesizer_result_t result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    result.result->deleted = true;

    return 0;
}

// Outputs oldest item
int oldestAction(synthesizer_array_t* list)
{
    synthesizer_result_t result = getOldest(*list);
    
    if (result.error != 0) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *(result.result));
    return 0;
}

const int menu_items_count = 9;
const action_t menu_items[] = {
    {"Vypsat data", print},
    {"Seradit podle jmena", sortByNameAction},
    {"Seradit podle roku vydani", sortByYearAction},
    {"Filtrovat", filterDialogueAction},
    {"Najit nejstarsi syntezator", oldestAction},
    {"Pridat syntezator", addItemAction},
    {"Najit syntezator podle id", searchDialogue},
    {"Smazat syntezator", deleteAction},
    {"Upravit syntezator", editAction},
};

/**
 * Prints menu to stdout
 */
void printMenu(void)
{
    printf("0: Ukoncit\n");
    for (int index = 0; index < menu_items_count; index++) {
        printf("%i: %s\n", index+1, menu_items[index].description);
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
        printMenu();
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
