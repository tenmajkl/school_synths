/**
 * Synths - types
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#ifndef TYPES_H
#define TYPES_H

#define DATA_FORMAT "%d,%s,%s,%d,%d,%d\n"
#define PRETTY_FORMAT "| %2d | %15s | %15s | %10d | %16d | %9d |\n"
#define INPUT_FORMAT "%d,%15[^,\n],%15[^,\n],%d,%d,%d\n"

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
 * Array of indexes of items from synth array.
 *
 * Its basicaly hack that allows us to return read-write array of specific data. It also takes less space, because its just integers
 */
typedef struct {
    int* array;
    int size;
    int capacity;
} synthesizer_index_array_t;

/**
 * Represents array of synths with capacity and size
 */
typedef struct {
    synthesizer_t* array; // this will be initialized only once - at loading, if we want to return only specific items
    synthesizer_index_array_t indexes; // indexes of curently used items 
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
    condition_t condition;
} synthesizer_field_t;

/**
 * Result of function that might return item field
 */
typedef struct {
    synthesizer_field_t result;
    int error;  
} synthesizer_field_result_t;

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
