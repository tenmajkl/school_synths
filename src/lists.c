// --- Lists and structures ---

#include "lists.h"
#include "types.h"
#include "comparing.h"
#include "helpers.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    array.array = malloc(list.size * sizeof(synthesizer_t*));

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
        synthesizer_t* new = realloc(array.array, array.size * sizeof(synthesizer_t*));
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
    input("Chces data jen vypsat (1), nebo i ulozit (2)? Pozn. pouze vypsani zabere vice pameti. ", 
          "%i", 
          &choice
    );
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
    input("Zadej id: ", "%d", &key.id);

    synthesizer_result_t result = search(*list, key, byIdCondition);

    if (result.error != 0) {
        return (synthesizer_result_t) { .error = result.error };
    }

    return (synthesizer_result_t) { result.result, 0 };
}
