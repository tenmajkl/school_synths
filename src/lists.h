#ifndef LISTS_H_
#define LISTS_H_

#include<stdio.h>
#include "types.h"

// --- Lists and structures ---

/**
 * Loads data from file to array
 *
 * Can return codes 5, 6, 0
 */
synthesizer_array_result_t load(FILE* input);

/**
 * Copies one list to another
 */
synthesizer_array_t copy(synthesizer_array_t list);

synthesizer_result_t get(synthesizer_array_t list, int index);

int push(synthesizer_array_t* list, synthesizer_t item);

// --- Output ---

/**
 * Outputs one item to dedicated file with given format string
 */
void writeOne(FILE* output, char* format, synthesizer_t item);

/** 
 * Outputs list to dedicated file with given format string
 */
void write(FILE* output, synthesizer_array_t array, char* format);

/**
 * Outputs head of table
 */
void printHead();

/**
 * Prints whole list to stdout with pretty format
 */
int print(synthesizer_array_t* list);

// --- Filtering ---

synthesizer_array_result_t filter(synthesizer_array_t list, synthesizer_t key, condition_t condition);

/**
 * Returns oldest synth
 */
synthesizer_result_t getOldest(synthesizer_array_t list);

// --- Sorting ---

/**
 * Merge sort melting
 */
void melt(synthesizer_array_t* list, synthesizer_array_t* help, int from, int to, int middle, condition_t compare);

/**
 * Merge sort algorithm
 */
void mergeSort(synthesizer_array_t* list, synthesizer_array_t* help, int from, int to, condition_t condition);

/**
 * Sorts data using merge sort
 *
 * Basicaly helper for sorting which makes it simpler to work with
 */
int sort(synthesizer_array_t* list, condition_t condition);

/**
 * Generic user interface for sorting
 */
int sortDialogue(synthesizer_array_t* list, condition_t condition);

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
synthesizer_result_t search(synthesizer_array_t list, synthesizer_t key, condition_t compare);

/**
 * Dialogue over searching
 */
synthesizer_result_t searchWithDialogue(synthesizer_array_t *list);

#endif
