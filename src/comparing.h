#ifndef COMPARING_H_
#define COMPARING_H_

#include "types.h"

/**
 * Compares two items by id
 */
int byIdCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by year
 */
int byYearCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by manufacturer
 */
int byManufacturerCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by name
 */
int byNameCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by number of voices
 */
int byVoicesCondition(synthesizer_t first, synthesizer_t second);

/**
 * Compares two items by analogness
 */
int byAnalogCondition(synthesizer_t first, synthesizer_t second);

#endif
