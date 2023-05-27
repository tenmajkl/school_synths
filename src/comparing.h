#ifndef COMPARING_H_
#define COMPARING_H_

#include "types.h"

/**
 * Compares two items by year
 *
 * @return int difference between year of first item and year of second item
 */
int byYearCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by manufacturer
 *
 * @return int difference between manufacturer of first item and manufacturer of second item
 */
int byManufacturerCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by name
 *
 * @return int difference between name of first item and name of second item
 */
int byNameCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by number of voices
 *
 * @return int difference between number of voices of first item and number of voices of second item
 */
int byVoicesCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by analogness
 *
 * @return int difference between analogness of first item and analogness of second item
 */
int byAnalogCondition(synthesizer_t first, synthesizer_t second);

#endif
