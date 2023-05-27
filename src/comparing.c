/**
 * Synths - comparing functions
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#include "comparing.h"
#include<string.h>

/**
 * Compares two items by year
 *
 * @return int difference between year of first item and year of second item
 */
int byYearCondition(synthesizer_t first, synthesizer_t second)
{
    return first.year - second.year;
}

/**
 * Compares two items by manufacturer
 *
 * @return int difference between manufacturer of first item and manufacturer of second item
 */
int byManufacturerCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.manufacturer, second.manufacturer);
}

/**
 * Compares two items by name
 *
 * @return int difference between name of first item and name of second item
 */
int byNameCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.name, second.name);
}

/**
 * Compares two items by number of voices
 *
 * @return int difference between number of voices of first item and number of voices of second item
 */
int byVoicesCondition(synthesizer_t first, synthesizer_t second)
{
    return first.voices - second.voices;
}

/**
 * Compares two items by analogness
 *
 * @return int difference between analogness of first item and analogness of second item
 */
int byAnalogCondition(synthesizer_t first, synthesizer_t second)
{
    return first.analog - second.analog;
}
