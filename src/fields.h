#ifndef FIELDS_H_
#define FIELDS_H_

#include "types.h"
#include "comparing.h"

/**
 * Fields which can be used for editing or grouping
 */
const int field_count = 5;
const synthesizer_field_t fields[] = {
    { NAME, "Podle jmena", "Jmeno", "%15s", byNameCondition },
    { MANUFACTURER, "Podle vyrobce", "Vyrobce", "%15s", byManufacturerCondition },
    { YEAR, "Podle roku vydani", "Rok vydani", "%d", byYearCondition },
    { VOICES, "Podle poctu hlasu", "Pocet hlasu", "%d", byVoicesCondition },
    { ANALOG, "Podle analogovosti", "Analogovy", "%d", byAnalogCondition }
};

/**
 * Outputs menu for filters
 */
void fieldsFilterMenu();

/**
 * Outputs menu for editing
 */
void fieldsEditMenu();

/**
 * Loads given value to given key from user 
 *
 * This allows us to dynamicaly edit and create keys for filtering
 */
void getKeyByField(synthesizer_array_t list, synthesizer_field_t field, synthesizer_t* key);

/**
 * Loads field from input
 */
synthesizer_field_tResult getField();

#endif
