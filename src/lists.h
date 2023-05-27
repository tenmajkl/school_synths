#ifndef LISTS_H_
#define LISTS_H_

#include<stdio.h>
#include "types.h"

#define BLOCK_LEN 16

/**
 * Loads data from file to array
 * 
 * @param FILE* input input file
 * @return synthesizer_array_result_t with codes 5, 6 or 0
 */
synthesizer_array_result_t load(FILE* input);

/**
 * Returns copy of given synth array
 *     The trick is that it actually copies just the indexes array and the actual data remains the same
 *
 * @param synthesizer_array_t list list to be copied
 * @return synthesizer_array_result_t with codes 5 or 0
 */
synthesizer_array_result_t copy(synthesizer_array_t list);

/**
 * Returns item from list at given index respecting array of indexes
 * 
 * @param synthesizer_array_t* list list which contains the item to be returned
 * @param int index index of the item to be returned
 * @return synthesizer_result_t with codes 1 or 0
 */
synthesizer_result_t get(synthesizer_array_t* list, int index);

// --- Output ---

/**
 * Outputs one item to dedicated file with given format string
 *
 * @param FILE* output output file
 * @param char* format format string
 * @param synthesizer_t item item to be printed
 */
void writeOne(FILE* output, char* format, synthesizer_t item);

/** 
 * Outputs list to dedicated file with given format string
 *
 * @param FILE* output output file
 * @param synthesizer_array_t array array to be printed
 * @param char* format format string
 */
void write(FILE* output, synthesizer_array_t array, char* format);

/**
 * Outputs head of table
 */
void printHead();

/**
 * Prints whole list to stdout with pretty format
 *
 * @param synthesizer_array_t* list list to be printed
 * @return int 0 - error code of success
 */
int print(synthesizer_array_t* list);

// --- Filtering ---

/**
 * Filters list by given condition and key and returns filtered values as array
 *
 * @param synthesizer_array_t list list to be filtered
 * @param synthesizer_t key key to be filtered by, only one field should be used
 * @param condition_t condition condition to be used for filtering, must compare the only filled field given synthesizer has
 * @return synthesizer_array_result_t array containing only filtered items with codes 0, 2 or 3
 */
synthesizer_array_result_t filter(synthesizer_array_t list, synthesizer_t key, condition_t condition);

/**
 * Extension of function filter with dialogue for choosing field and condition along with creating key item
 *
 * @param synthesizer_array_t* list list to be filtered
 * @return synthesizer_array_result_t array containing only filtered items with codes 0, 2, 3 or 9
 */
synthesizer_array_result_t filterWithDialogue(synthesizer_array_t* list);

/**
 * Returns oldest synth
 *
 * @param synthesizer_array_t list list to be searched
 * @return synthesizer_result_t oldest synthesizer with codes 0 or 2
 */
synthesizer_result_t getOldest(synthesizer_array_t list);

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
void melt(synthesizer_array_t* list, synthesizer_index_array_t* help, int from, int to, int middle, condition_t compare);

/**
 * Merge sort algorithm for sorting indexes by values on those indexes
 *
 * @param synthesizer_array_t* list list to be sorted
 * @param synthesizer_index_array_t* help heling array for sorting
 * @param int from starting index
 * @param int to ending index
 * @param condition_t condition condition to be used for sorting
 */
void mergeSort(synthesizer_array_t* list, synthesizer_index_array_t* help, int from, int to, condition_t condition);

/**
 * Sorts indexes of list by given condition.
 *     Its based on function mergeSort, but creates helper array for sorting and fills other arguments of the function.
 *
 * @param synthesizer_array_t* list list to be sorted
 * @param condition_t condition condition to be used for sorting
 * @return int codes 0 or 3
 */
int sort(synthesizer_array_t* list, condition_t condition);

#endif
