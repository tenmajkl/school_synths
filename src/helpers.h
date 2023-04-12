#ifndef ERRORS_H_
#define ERRORS_H_

/* --- Errors ---
 * In order to make errors more consistent, they are assigned to codes.
 */

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

#endif
