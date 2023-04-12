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

#endif
