#ifndef ERRORS_H_
#define ERRORS_H_

#define COLOR(C, CODE) printf("\033[%im", 30 + C); CODE; printf("\033[0m");

/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

extern const char errors[][64];

/**
 * Outputs error message of given code
 *
 * @param code Code of error from errors array
 */
void error(int code);

// --- Helpers ---

/**
 * Clears screen depending on curent OS
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
 * Loads single value from user, even if it can't
 */
void input(char* message, char* format, void* pointer);

#endif
