/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

#include "helpers.h"
#include "types.h"
#include<stdio.h>
#include<stdlib.h>

const char errors[][64] = {
    "Nelze pridat prvek, v pameti je malo mista.",
    "Pole je prazdne",
    "Nepodarilo se vytvorit pomocne pole.",
    "Spatna volba",
    "Doslo misto v pameti!",
    "Chyba formatu",
    "Nepodarilo se najit prvek v poli",
    "Neznama polozka",
};

/**
 * Outputs error message of given code
 *
 * @param code Code of error from errors array
 */
void error(int code)
{
    COLOR(RED, 
        printf("[ERROR] %s\n", errors[code - 1])
    );
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

/**
 * Loads single value from user, even if it can't
 */
void input(char* message, char* format, void* pointer)
{
    int code;
    do {
        clearBuffer();
        printf("%s", message);
        code = scanf(format, pointer);
    } while (code != 1);
}
