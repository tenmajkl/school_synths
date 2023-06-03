/**
 * Synths - lists
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#include "lists.h"
#include "fields.h"
#include "types.h"
#include "comparing.h"
#include "helpers.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 * Loads data from file to array
 * 
 * @param FILE* input input file
 * @return synthesizer_array_result_t with codes 5, 6 or 0
 */
synthesizer_array_result_t load(FILE* input)
{
    synthesizer_array_result_t result;
    result.error = 0; 
    synthesizer_array_t array;
    array.array = malloc(BLOCK_LEN * sizeof(synthesizer_t));
    if (array.array == NULL) {
        result.result = array;
        result.error = 5;
        return result;
    }

    array.size = 0;
    array.capacity = BLOCK_LEN;
    array.indexes.array = malloc(BLOCK_LEN * sizeof(int));
    if (array.indexes.array == NULL) {
        free(array.array);
        result.result = array;
        result.error = 5;
        return result;
    }

    array.indexes.size = 0;
    array.indexes.capacity = BLOCK_LEN;
    synthesizer_t* new;
    int* new_indexes;
    int analog;
    int loading_result;

    while ((loading_result = fscanf(input, INPUT_FORMAT,
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
        array.indexes.array[array.indexes.size] = array.size;
        array.size++;
        array.indexes.size++;
        if (array.size == array.capacity) {
            array.capacity += BLOCK_LEN;
            new = realloc(array.array, array.capacity * sizeof(synthesizer_t));
            if (new == NULL) {
                result.result = array;
                result.error = 5;
                return result;
            }
            array.array = new;

            array.indexes.capacity += BLOCK_LEN; 
            new_indexes = realloc(array.indexes.array, array.capacity * sizeof(synthesizer_t));
            if (new == NULL) {
                result.result = array;
                result.error = 5;
                return result;
            }
            array.indexes.array = new_indexes;
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
 * Returns copy of given synth array
 *     The trick is that it actually copies just the indexes array and the actual data remains the same
 *
 * @param synthesizer_array_t list list to be copied
 * @return synthesizer_array_result_t with codes 5 or 0
 */
synthesizer_array_result_t copy(synthesizer_array_t list)
{
    synthesizer_array_t result = list;
    int size = list.indexes.size * sizeof(int);
    int* new = malloc(size);
    if (new == NULL) {
        return (synthesizer_array_result_t) { .error = 5 };
    }
    result.indexes.array = new;
    memcpy(result.indexes.array, list.indexes.array, size);

    return (synthesizer_array_result_t) { result, 0 };
}

/**
 * Returns item from list at given index respecting array of indexes
 * 
 * @param synthesizer_array_t* list list which contains the item to be returned
 * @param int index index of the item to be returned
 * @return synthesizer_result_t with codes 1 or 0
 */
synthesizer_result_t get(synthesizer_array_t* list, int index)
{
    if (index < 0 || index >= list->indexes.size) {
        return (synthesizer_result_t) { .error = 1 };
    }
    return (synthesizer_result_t) { &(list->array[list->indexes.array[index]]), 0 };
}

// --- Output ---

/**
 * Outputs one item to dedicated file with given format string
 *
 * @param FILE* output output file
 * @param char* format format string
 * @param synthesizer_t item item to be printed
 */
void writeOne(FILE* output, char* format, synthesizer_t item)
{
    fprintf(output, format, item.id, item.name, item.manufacturer, item.year, item.voices, item.analog ? 1 : 0);
}

/** 
 * Outputs list to dedicated file with given format string
 *
 * @param FILE* output output file
 * @param synthesizer_array_t array array to be printed
 * @param char* format format string
 */
void write(FILE* output, synthesizer_array_t array, char* format)
{
    synthesizer_t item;
    for (int index = 0; index < array.indexes.size; index++) {
        item = *get(&array, index).result;
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
 *
 * @param synthesizer_array_t* list list to be printed
 * @return int 0 - error code of success
 */
int print(synthesizer_array_t* list)
{
    printHead();
    write(stdout, *list, PRETTY_FORMAT);
    return 0;
}

// --- Filtering ---

/**
 * Filters list by given condition and key and returns filtered values as array
 *
 * @param synthesizer_array_t list list to be filtered
 * @param synthesizer_t key key to be filtered by, only one field should be used
 * @param condition_t condition condition to be used for filtering, must compare the only filled field given synthesizer has
 * @return synthesizer_array_result_t array containing only filtered items with codes 0, 2 or 3
 */
synthesizer_array_result_t filter(synthesizer_array_t list, synthesizer_t key, condition_t condition)
{
    synthesizer_array_result_t result;
    result.error = 0;
    synthesizer_array_t array = list;
    array.indexes.capacity = list.indexes.capacity;
    array.indexes.size = 0;
    array.indexes.array = malloc(list.indexes.capacity * sizeof(int));

    if (array.array == NULL) {
        result.error = 2;
        result.result = list;
        free(array.indexes.array);
        return result;
    }

    for (int index = 0; index < list.size; index++) {
        if (!list.array[index].deleted && condition(list.array[index], key) == 0) {
            array.indexes.array[array.indexes.size] = index;
            array.indexes.size++;
        }
    }

    if (array.size < list.size) {
        int* new = realloc(array.indexes.array, array.indexes.size * sizeof(int));
        if (new == NULL) {
            result.error = 3;
            result.result = array;
            return result;
        }
        array.indexes.array = new;
        array.indexes.capacity = array.indexes.size;
    }

    result.result = array;
    return result;
}

/**
 * Extension of function filter with dialogue for choosing field and condition along with creating key item
 *
 * @param synthesizer_array_t* list list to be filtered
 * @return synthesizer_array_result_t array containing only filtered items with codes 0, 2, 3 or 9
 */
synthesizer_array_result_t filterWithDialogue(synthesizer_array_t* list)
{
    fieldsFilterMenu();
    synthesizer_field_result_t field = getField();
    if (field.error != 0) {
        return (synthesizer_array_result_t) { .error = field.error };
    }

    synthesizer_t key;
    getKeyByField(field.result, &key);

    return filter(*list, key, field.result.condition); 
}

/**
 * Returns oldest synth
 *
 * @param synthesizer_array_t list list to be searched
 * @return synthesizer_result_t oldest synthesizer with codes 0 or 2
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
 * Merge sort melting function.
 *    It should be called only from mergeSort function.
 *
 * @param synthesizer_array_t* list list to be sorted
 * @param synthesizer_index_array_t* help helper array for sorting
 * @param int from start index
 * @param int to end index
 * @param int middle middle index
 * @param condition_t compare function for comparing two items
 */
void melt(synthesizer_array_t* list, synthesizer_index_array_t* help, int from, int to, int middle, condition_t compare)
{
    int left = from, right = middle + 1, index = from;
    while (left <= middle && right <= to) {
        if (compare(*get(list, left).result, *get(list, right).result) <= 0) {
            help->array[index++] = list->indexes.array[left++];
        } else {
            help->array[index++] = list->indexes.array[right++];
        }
    }

    while (left <= middle) {
        help->array[index++] = list->indexes.array[left++];
    }

    while (right <= to) {
        help->array[index++] = list->indexes.array[right++];
    }
}

/**
 * Merge sort algorithm for sorting indexes by values on those indexes
 *
 * @param synthesizer_array_t* list list to be sorted
 * @param synthesizer_index_array_t* help heling array for sorting
 * @param int from starting index
 * @param int to ending index
 * @param condition_t condition condition to be used for sorting
 */
void mergeSort(synthesizer_array_t* list, synthesizer_index_array_t* help, int from, int to, condition_t condition)
{
    if (from >= to) {
        return;
    }

    int middle = (from + to) / 2;
    mergeSort(list, help, from, middle, condition);
    mergeSort(list, help, middle + 1, to, condition);

    melt(list, help, from, to, middle, condition);

    while (from <= to) {
        list->indexes.array[from] = help->array[from];
        from++;
    }
}

/**
 * Sorts indexes of list by given condition.
 *     Its based on function mergeSort, but creates helper array for sorting and fills other arguments of the function.
 *
 * @param synthesizer_array_t* list list to be sorted
 * @param condition_t condition condition to be used for sorting
 * @return int codes 0 or 3
 */
int sort(synthesizer_array_t* list, condition_t condition)
{
    synthesizer_index_array_t help;;
    help.array = malloc(list->indexes.size * sizeof(int));
    if (help.array == NULL) {
        return 3;
    }
    mergeSort(list, &help, 0, list->indexes.size - 1, condition);
    free(help.array);
    return 0;
}
