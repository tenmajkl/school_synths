// --- Comparing functions ---

#include "comparing.h"
#include<string.h>

/**
 * Compares two items by id
 */
int byIdCondition(synthesizer_t first, synthesizer_t second)
{
    return first.id - second.id;
}

/**
 * Compares two items by year
 */
int byYearCondition(synthesizer_t first, synthesizer_t second)
{
    return first.year - second.year;
}

/**
 * Compares two items by manufacturer
 */
int byManufacturerCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.manufacturer, second.manufacturer);
}

/**
 * Compares two items by name
 */
int byNameCondition(synthesizer_t first, synthesizer_t second)
{
    return strcmp(first.name, second.name);
}

/**
 * Compares two items by number of voices
 */
int byVoicesCondition(synthesizer_t first, synthesizer_t second)
{
    return first.voices - second.voices;
}

/**
 * Compares two items by analogness
 */
int byAnalogCondition(synthesizer_t first, synthesizer_t second)
{
    return first.analog - second.analog;
}
