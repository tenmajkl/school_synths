#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define DATA_FORMAT "%d|%s|%s|%d|%d|%d\n"
#define PRETTY_FORMAT "| %2d | %15s | %15s | %10d | %16d | %9d |\n"

// Represents synth entity
typedef struct {
    int id;
    char name[16];
    char manufacturer[16];
    int year;
    int voices;
    bool analog;
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
} MenuItem;

// Sort comparing function
typedef bool (*Condition)(Synthesizer first, Synthesizer second);

const char errors[][64] = {
    "Nelze pridat prvek, v pameti je malo mista.",
    "Pole je prazdne",
    "Nepodarilo se vytvorit pomocne pole.",
    "Spatna volba",
    "Doslo misto v pameti!",
    "Chyba formatu"
};

// Outputs error message of given code
void error(int code)
{
    printf("[ERROR] %s\n", errors[code]);
}

// Clears screen
void clear(void)
{
    #if defined __WIN32 || defined __WIN64
        system("cls"); // Windows has different command for clearing.
    #else
        system("clear"); // If the os is not windows its propably unix-like.
    #endif
}

// Clears stdin buffer
void clearBuffer(void)
{
    while ((getchar()) != '\n');
}

// Waits for user to press key
void waitForClick(void)
{
    clearBuffer();
    puts("Zmackni enter pro pokracovani");
    getchar();
}

// Loads data from file
SynthesizerArray load(FILE* input, int* error)
{
    SynthesizerArray array;
    array.array = malloc(16 * sizeof(Synthesizer));
    array.size = 0;
    array.capacity = 16;
    Synthesizer* new;
    int analog;
    int loading_result;

    while ((loading_result = fscanf(input, "%d|%15[^|]|%15[^|]|%d|%d|%d",
                &array.array[array.size].id,
                array.array[array.size].name,
                array.array[array.size].manufacturer,
                &array.array[array.size].year,
                &array.array[array.size].voices,
                &analog
            )) == 6
    ) {
        array.array[array.size].analog = analog != 0;
        array.size++;
        if (array.size == array.capacity) {
            array.capacity += 16;
            new = realloc(array.array, array.capacity * sizeof(Synthesizer));
            if (new == NULL) {
                *error = 4;
                return array;
            }
            array.array = new;
        }
    }

    if (loading_result != 6) {
        *error = 5;
        return array;
    }

    return array;
}

// Copies one list to another
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

// Outputs one item to dedicated file with given format string
void writeOne(FILE* output, char* format, Synthesizer item)
{
    fprintf(output, format, item.id, item.name, item.manufacturer, item.year, item.voices, item.analog ? 1 : 0);
}

// Outputs list to dedicated file with given format string
void write(FILE* output, SynthesizerArray array, char* format)
{
    Synthesizer item;
    for (int index = 0; index < array.size; index++) {
        item = array.array[index];
        writeOne(output, format, item);
    }
}

// Outputs head of table
void printHead()
{
    puts(
        "+----+-----------------+-----------------+------------+------------------+-----------+\n"
        "| ID | Model           | Vyrobce         | Rok vydani | Pocet oscilatoru | Analogovy |\n"
        "|----+-----------------+-----------------+------------+------------------+-----------|"
    );
}

// Prints whole list to stdout with pretty format
int print(SynthesizerArray* list)
{
    printHead();
    write(stdout, *list, PRETTY_FORMAT);
    return 0;
}

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
    item.id = list->size > 1 ? list->array[list->size - 2].id + 1 : 1; // TODO when implementing deletion, create some last() function

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
// TODO make separate functions
int onlyManufacturerModels(SynthesizerArray* list)
{
    char manufacturer[16];
    printf("Zadej vyrobce: ");
    scanf("%15s", manufacturer);

    clear();
    printHead();

    for (int index = 0; index < list->size; index++) {
        if (strcmp(list->array[index].manufacturer, manufacturer) == 0) {
            writeOne(stdout, PRETTY_FORMAT, list->array[index]);
        }
    }

    return 0;
}

// Outputs oldest item
// TODO separate
int oldest(SynthesizerArray* list)
{
    if (list->size == 0) {
        return NULL;
    }
    Synthesizer oldest = list->array[0];
    for (int index = 1; index < list->size; index++) {
        if (list->array[index].year < oldest.year) {
            oldest = list->array[index];
        }
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, oldest);
    return 0;
}

// Part of merge sort - melting
void melt(SynthesizerArray* list, SynthesizerArray* help, int from, int to, int middle, Condition compare)
{
    int left = from, right = middle + 1, index = from;
    while (left <= middle && right <= to) {
        if (compare(list->array[left], list->array[right])) {
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

// Sorting condition for smallest year
bool byYearCondition(Synthesizer first, Synthesizer second)
{
    return first.year <= second.year;
}

// Sorting condition for alphabetic sorting
bool byNameCondition(Synthesizer first, Synthesizer second)
{
    return strcmp(first.name, second.name) <= 0;
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


const int menu_items_count = 6;
const MenuItem menu_items[] = {
    {"Vypsat data", print},
    {"Seradit podle jmena", sortByName},
    {"Seradit podle roku vydani", sortByYear},
    {"Vypsat modely urcite znacky", onlyManufacturerModels},
    {"Najit nejstarsi syntezator", oldest},
    {"Pridat syntezator", addItem}
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
            error(errors[3]);
            continue;
        }

        int code;
        if ((code = menu_items[choice - 1].function(list)) != 0) {
            error(code - 1);
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
    int code = -1;
    SynthesizerArray list = load(file, &code);

    if (code > -1) {
        error(code);
        waitForClick();
    }
    fclose(file);

    menu(&list);

    clearBuffer();
    char save;
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

    free(list.array);
    return 0;
}
