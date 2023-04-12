/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

#include "helpers.h"
#include<stdio.h>
#include<stdlib.h>

/**
 * Outputs error message of given code
 *
 * @param code Code of error from errors array
 */
void error(int code)
{
    printf("[ERROR] %s\n", errors[code - 1]);
}

// --- Helpers ---

/**
 * Clears screen depending on curent OS
 */
void clear(void)
{
    #if defined __WIN32 || defined __WIN64
        system("cls"); // Windows has different command for clearing.
    #else
        system("clear"); // If the os is not windows its propably unix-like.
    #endif
}

/**
 * Clears buffer of stdin which is used when reading input
 */
void clearBuffer(void)
{
    while ((getchar()) != '\n');
}

/**
 * Waits for user to click before new actions
 */
void waitForClick(void)
{
    clearBuffer();
    puts("Zmackni enter pro pokracovani");
    getchar();
}
