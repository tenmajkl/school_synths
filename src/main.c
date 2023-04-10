#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define DATA_FORMAT "%s|%s|%d|%d|%d\n"
#define PRETTY_FORMAT "| %15s | %15s | %10d | %16d | %9d |\n"

/**
 * Structures 
 */

// Represents synth entity
typedef struct {
    char name[16];
    char manufacturer[16];
    int year;
    int voices;
    bool analog;
    bool deleted;
} Synthesizer;

// Represents array of synths with capacity and size
typedef struct {
    Synthesizer* array;
    int size;
    int capacity;
} SynthesizerArray;

// Represents one item from menu
typedef struct {
    char* description;
    int (*function)(SynthesizerArray* list);
} action_t;

/**
 * Structure containing result of SynthesizerArray along with error code in case of error
 *
 * Basicaly cheap copy of rust Result
 */
typedef struct {
    SynthesizerArray result;
    int error;
} SynthesizerArrayResult;

/**
 * Structure containing result of Synthesizer along with error code in case of error
 *
 * Basicaly cheap copy of rust Result
 */
typedef struct {
    Synthesizer *result;
    int error;
} SynthesizerResult;

// Sort comparing function
typedef int (*Condition)(Synthesizer first, Synthesizer second);

typedef enum {
    NAME,
    MANUFACTURER,
    YEAR,
    VOICES,
    ANALOG
} SynthesizerFieldIndex;

typedef struct {
    SynthesizerFieldIndex index;
    char* filter_description;
    char* edit_description;
    char* scanf_format;
    Condition condition;
} SynthesizerField;

/**
 * Errors
 *
 * In order to make errors more consistent, they are assigned to codes.
 */

const char errors[][64] = {
    "Nelze pridat prvek, v pameti je malo mista.",
    "Pole je prazdne",
    "Nepodarilo se vytvorit pomocne pole.",
    "Spatna volba",
    "Doslo misto v pameti!",
    "Chyba formatu",
    "Nepodarilo se najit prvek v poli"
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

/**
 * Helpers
 */

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

/**
 * Lists and structures
 */

/**
 * Loads data from file to array
 *
 * Can return codes 5, 6, 0
 */
SynthesizerArrayResult load(FILE* input)
{
    SynthesizerArray array;
    array.array = malloc(16 * sizeof(Synthesizer));
    array.size = 0;
    array.capacity = 16;
    Synthesizer* new;
    int analog;
    int loading_result;

    while ((loading_result = fscanf(input, "%15[^|]|%15[^|]|%d|%d|%d\n",
                array.array[array.size].name,
                array.array[array.size].manufacturer,
                &array.array[array.size].year,
                &array.array[array.size].voices,
                &analog
            )) == 5
    ) {
        array.array[array.size].analog = analog != 0;
        array.array[array.size].deleted = false;
        array.size++;
        if (array.size == array.capacity) {
            array.capacity += 16;
            new = realloc(array.array, array.capacity * sizeof(Synthesizer));
            if (new == NULL) {
                return (SynthesizerArrayResult) { array, 5 };
            }
            array.array = new;
        }
    }

    if (loading_result != EOF) {
        return (SynthesizerArrayResult) { array, 6 };
    }

    return (SynthesizerArrayResult) { array, 0 };
}

/**
 * Copies one list to another
 */
SynthesizerArray copy(SynthesizerArray list)
{
    SynthesizerArray result = list;
    Synthesizer* array = malloc(list.size * sizeof(Synthesizer));

    for (int i = 0; i < list.size; i++) {
        array[i] = list.array[i];
    }

    result.array = array;
    return result;
}

/**
 * Output
 */

/**
 * Outputs one item to dedicated file with given format string
 */
void writeOne(FILE* output, char* format, Synthesizer item)
{
    fprintf(output, format, item.name, item.manufacturer, item.year, item.voices, item.analog ? 1 : 0);
}

/** 
 * Outputs list to dedicated file with given format string
 */
void write(FILE* output, SynthesizerArray array, char* format)
{
    Synthesizer item;
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
        "+-----------------+-----------------+------------+------------------+-----------+\n"
        "| Model           | Vyrobce         | Rok vydani | Pocet oscilatoru | Analogovy |\n"
        "|-----------------+-----------------+------------+------------------+-----------|"
    );
}

/**
 * Prints whole list to stdout with pretty format
 */
int print(SynthesizerArray* list)
{
    printHead();
    write(stdout, *list, PRETTY_FORMAT);
    return 0;
}



SynthesizerArrayResult filter(SynthesizerArray list, Synthesizer key, Condition condition)
{
    SynthesizerArray result;
    result.array = malloc(list.size * sizeof(Synthesizer));

    if (result.array == NULL) {
        return (SynthesizerArrayResult) { list, 3 };
    }

    int result_index = 0;

    for (int index = 0; index < list.size; index++) {
        if (condition(list.array[index], key) == 0) {
            result.array[result_index] = list.array[index];
            result_index++;
        }
    }

    if (result_index < list.size) {
        Synthesizer* new = realloc(result.array, result_index * sizeof(Synthesizer));
        if (new == NULL) {
            return (SynthesizerArrayResult) { result, 3 };
        }
        result.array = new; 
    }

    return (SynthesizerArrayResult) { result, 0 };
}


// Sorting condition for smallest year
int byYearCondition(Synthesizer first, Synthesizer second)
{
    return first.year - second.year;
}

int byManufacturerCondition(Synthesizer first, Synthesizer second)
{
    return strcmp(first.manufacturer, second.manufacturer);
}

// Sorting condition for alphabetic sorting
int byNameCondition(Synthesizer first, Synthesizer second)
{
    return strcmp(first.name, second.name);
}

int byVoicesCondition(Synthesizer first, Synthesizer second)
{
    return first.voices - second.voices;
}

int byAnalogCondition(Synthesizer first, Synthesizer second)
{
    return first.analog - second.analog;
}

const int field_count = 5;
const SynthesizerField fields[] = {
    { NAME, "Podle jmena", "Jmeno", "%15s", byNameCondition },
    { MANUFACTURER, "Podle vyrobce", "Vyrobce", "%15s", byManufacturerCondition },
    { YEAR, "Podle roku vydani", "Rok vydani", "%d", byYearCondition },
    { VOICES, "Podle poctu hlasu", "Pocet hlasu", "%d", byVoicesCondition },
    { ANALOG, "Podle analogovosti", "Analogovy", "%d", byAnalogCondition }
};

void fieldsFilterMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].filter_description);
    }
}

void fieldsEditMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].edit_description);
    }
}

// Every key is represented by Synthesizer struct, here we get this struct
void getKeyByField(SynthesizerArray list, SynthesizerField field, Synthesizer* key)
{
    printf("Zadej %s: ", field.edit_description);
    clearBuffer();

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
            clearBuffer();
            char analog;
            printf("Je analogovy? y/n: ");
            scanf("%c", &analog);
            key->analog = analog == 'y';
            break;
    }

}

SynthesizerField getField()
{
    int index;
    printf("Zadej cislo polozky: ");
    scanf("%d", &index);
    return fields[index];
}



SynthesizerResult getOldest(SynthesizerArray list)
{
    if (list.size == 0) {
        return (SynthesizerResult) { .error = 2 };
    }
    Synthesizer oldest = list.array[0];
    for (int index = 1; index < list.size; index++) {
        if (list.array[index].year < oldest.year) {
            oldest = list.array[index];
        }
    }

    return (SynthesizerResult) { &oldest, 0 };
}



// Part of merge sort - melting
void melt(SynthesizerArray* list, SynthesizerArray* help, int from, int to, int middle, Condition compare)
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

// Merge sort itself
void mergeSort(SynthesizerArray* list, SynthesizerArray* help, int from, int to, Condition condition)
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

// Abstraction over mergeSort that creates help array
int sort(SynthesizerArray* list, Condition condition)
{
    SynthesizerArray help = *list;
    help.array = malloc(help.size * sizeof(Synthesizer));
    if (help.array == NULL) {
        return 3;
    }
    mergeSort(list, &help, 0, list->size - 1, condition);
    free(help.array);
    return 0;
}

SynthesizerResult binarySearch(SynthesizerArray list, int from, int to, Synthesizer key, Condition condition)
{
    if (from > to) {
        return (SynthesizerResult) { .error = 7 };
    }

    int middle = (to + from) / 2;

    if (condition(list.array[middle], key) == 0) {
        return (SynthesizerResult) { &list.array[middle], 0 };
    }

    if (condition(key, list.array[middle]) < 0) {
        return binarySearch(list, from, middle - 1, key, condition);
    }
    
    return binarySearch(list, middle + 1, to, key, condition);
}

// Searches with dialogue
SynthesizerResult searchWithDialogue(SynthesizerArray *list)
{
    Synthesizer key;;
    printf("Zadej jmeno: ");
    scanf("%15s", key.name);

    return binarySearch(*list, 0, list->size - 1, key, byNameCondition);
}

// User interface for sorting
int sortDialogue(SynthesizerArray* list, Condition condition)
{
    int choice;
    printf("Chces data jen vypsat (1), nebo i ulozit (2)? Pozn. pouze vypsani zabere vice pameti. ");
    scanf("%i", &choice);
    if (choice == 1) {
        SynthesizerArray copied = copy(*list);
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

/**
 * Actions from menu
 */

// Adds item to list
int addItem(SynthesizerArray* list)
{
    Synthesizer* new;
    if (list->size == list->capacity) {
        list->capacity++;
        new = realloc(list->array, list->size);
        if (new == NULL) {
            return 1;
        }
        list->array = new;
    }
    list->size++;

    Synthesizer item;

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

    list->array[list->size - 1] = item;
    return 0;
}

// Prints models from loaded manufacturer
int filterDialogue(SynthesizerArray* list)
{ 
    fieldsFilterMenu();
    SynthesizerField field = getField();
    Synthesizer key;
    getKeyByField(*list, field, &key);
    printHead();

    SynthesizerArrayResult models = filter(*list, key, field.condition);
    if (models.error != 0) {
        return models.error;
    }
    
    write(stdout, models.result, PRETTY_FORMAT);

    free(models.result.array);

    return 0;
}

// Menu item for sorting by year
int sortByYear(SynthesizerArray* list)
{
    return sortDialogue(list, byYearCondition);
}

// Menu item for sorting by name
int sortByName(SynthesizerArray* list)
{
    return sortDialogue(list, byNameCondition);
}

int searchDialogue(SynthesizerArray *list)
{
    SynthesizerResult result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *result.result);
    return 0;
}


int edit(SynthesizerArray* list)
{
    SynthesizerResult result = searchWithDialogue(list);

    if (result.error != 0) {
        return result.error;
    }

    Synthesizer* item = result.result;

    fieldsEditMenu();
    SynthesizerField field = getField(); 
    getKeyByField(*list, field, item);

    return 0;
}

int delete(SynthesizerArray *list)
{
    SynthesizerResult result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    result.result->deleted = true;

    return 0;
}

// Outputs oldest item
int oldest(SynthesizerArray* list)
{
    SynthesizerResult result = getOldest(*list);
    
    if (result.error != -1) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *result.result);
    return 0;
}

const int menu_items_count = 9;
const action_t menu_items[] = {
    {"Vypsat data", print},
    {"Seradit podle jmena", sortByName},
    {"Seradit podle roku vydani", sortByYear},
    {"Filtrovat", filterDialogue},
    {"Najit nejstarsi syntezator", oldest},
    {"Pridat syntezator", addItem},
    {"Najit syntezator podle jmena", searchDialogue},
    {"Smazat syntezator", delete},
    {"Upravit syntezator", edit},
};

// Prints menu to stdout
void printMenu(void)
{
    printf("0: Ukoncit\n");
    for (int index = 0; index < menu_items_count; index++) {
        printf("%i: %s\n", index+1, menu_items[index].description);
    }
}

// Menu interface
void menu(SynthesizerArray* list)
{
    int choice = 0;
    do {
        clear();
        printMenu();
        clearBuffer();
        printf("Zadej moznost: ");
        scanf("%i", &choice);
        if (choice == 0) {
            return;
        }

        if (choice < 0 || choice > menu_items_count) {
            error(4);
            continue;
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
    // TODO think of using cli args
    char file_name[32];
    printf("Zadej jmeno souboru: ");
    scanf("%31s", file_name);

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return -1;
    }
    SynthesizerArrayResult loaded = load(file);

    if (loaded.error != 0) {
        error(loaded.error);
        waitForClick();
    }

    SynthesizerArray list = loaded.result;

    fclose(file);

    menu(&list);

    char save;
    do {
        clearBuffer();
        printf("Ulozit zmeny? y/n: ");
        scanf("%c", &save);
        if (save == 'y') {
            printf("Zadej jmeno souboru: ");
            scanf("%31s", file_name);
            FILE* file = fopen(file_name, "w");
            if (file == NULL) {
                free(list.array);
                return -1;
            }

            write(file, list, DATA_FORMAT);

            fclose(file);
        }
    } while (save != 'n');

    free(list.array);
    return 0;
}
