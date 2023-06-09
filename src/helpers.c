/**
 * Synths - helpers
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#include "helpers.h"
#include "types.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

/**
 * Error messages
 */
const char errors[][64] = {
    "Nelze pridat prvek - v pameti je malo mista",
    "Pole je prazdne",
    "Nepodarilo se vytvorit pomocne pole",
    "Spatna volba",
    "Doslo misto v pameti!",
    "Chyba formatu",
    "Nepodarilo se najit prvek v poli",
    "Neznama polozka",
    "Zastaveno uzivatelem", // dont print
    "Soubor neexistuje",
};

/**
 * Printf with colors.
 *
 * @param color_t color color the output text will have
 */
void printfc(color_t color, const char* format, ...)
{
    printf("\033[%dm", 30 + color);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\033[0m");
}

/**
 * Outputs error message of given code
 *
 * @param code code of error from errors array
 */
void error(int code)
{
    printfc(RED, "[ERROR] %s\n", errors[code - 1]);
}

// --- Helpers ---

/**
 * Clears screen
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
//    while (getchar() != '\n');
    fflush(stdin);
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
 * Loads single value from user, when the value is not valid, it tries again
 *
 * @param char* message prompt that wil be printed before loading
 * @param char* format loading format
 * @param void* pointer variable which will be loaded
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
