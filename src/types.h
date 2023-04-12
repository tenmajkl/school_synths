/**
 * Types
 *
 * This file contains all types used in program
 *
 * Why?
 * Because it was easiest to split from main.c and its more organized
 */

#ifndef TYPES_H
#define TYPES_H

#define DATA_FORMAT "%d|%s|%s|%d|%d|%d\n"
#define PRETTY_FORMAT "| %2d | %15s | %15s | %10d | %16d | %9d |\n"

#include<stdbool.h>

/**
 * Represents synth entity
 */
typedef struct {
    int id;
    char name[16];
    char manufacturer[16];
    int year;
    int voices;
    bool analog;
    bool deleted;
} synthesizer_t;

/**
 * Represents array of synths with capacity and size
 */
typedef struct {
    synthesizer_t* array;
    int size;
    int capacity;
} synthesizer_array_t;

/**
 * Represents one item from menu
 */
typedef struct {
    char* description;
    int (*function)(synthesizer_array_t* list);
} action_t;

/**
 * Structure containing result of synthesizer_array_t along with error code in case of error
 *
 * Basicaly cheap copy of rust Result
 */
typedef struct {
    synthesizer_array_t result;
    int error;
} synthesizer_array_result_t;

/**
 * Structure containing result of synthesizer_t along with error code in case of error
 *
 * Basicaly cheap copy of rust Result
 */
typedef struct {
    synthesizer_t *result;
    int error;
} synthesizer_result_t;

/**
 * Comparing function
 */
typedef int (*condition_t)(synthesizer_t first, synthesizer_t second);

/**
 * Type of item field
 */
typedef enum {
    NAME,
    MANUFACTURER,
    YEAR,
    VOICES,
    ANALOG
} synthesizer_field_index_t;

/**
 * Represents item field for dynamic actions
 *
 * Each field has its index to be represented in getField,
 * descriptions for field menus,
 * format for scanf
 * and condition function for comparing
 */
typedef struct {
    synthesizer_field_index_t index;
    char* filter_description;
    char* edit_description;
    char* scanf_format;
    condition_t condition;
} synthesizer_field_t;

/**
 * Result of function that might return item field
 */
typedef struct {
    synthesizer_field_t result;
    int error;  
} synthesizer_field_tResult;

typedef enum {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7,
} color_t;

#endif
