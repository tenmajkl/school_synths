#ifndef ERRORS_H_
#define ERRORS_H_

/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

#include "types.h"
extern const char errors[][64];

/**
 * Printf with colors.
 *
 * @param color_t color color the output text will have
 */
void printfc(color_t color, const char* format, ...);

/**
 * Outputs error message of given code
 *
 * @param code code of error from errors array
 */
void error(int code);

// --- Helpers ---

/**
 * Clears screen 
 */
void clear(void);

/**
 * Clears buffer of stdin which is used when reading input
 */
void clearBuffer(void);

/**
 * Waits for user to click before new actions
 */
void waitForClick(void);

/**
 * Loads single value from user, when the value is not valid, it tries again
 *
 * @param char* message prompt that wil be printed before loading
 * @param char* format loading format
 * @param void* pointer variable which will be loaded
 */
void input(char* message, char* format, void* pointer);

#endif
