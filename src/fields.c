#include "fields.h"
#include "types.h"
#include "helpers.h"
#include "comparing.h"
#include<stdio.h>

// --- Fields ---

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
void fieldsFilterMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].filter_description);
    }
}

/**
 * Outputs menu for editing
 */
void fieldsEditMenu()
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index, fields[index].edit_description);
    }
}

/**
 * Loads given value to given key from user 
 *
 * This allows us to dynamicaly edit and create keys for filtering
 */
void getKeyByField(synthesizer_array_t list, synthesizer_field_t field, synthesizer_t* key)
{
    printf("Zadej %s: ", field.edit_description);
    clearBuffer();
    char analog;
    
    switch (field.index) {
        case NAME:
            input("", "%15s", key->name);
            break;
        case MANUFACTURER:
            input("", "%15s", key->manufacturer);
            break;
        case YEAR:
            input("", "%d", &key->year);
            break;
        case VOICES:
            input("", "%d", &key->voices);
            break;
        case ANALOG:
            input("", "%c", &analog);
            key->analog = analog == 'y';
            break;
    }

}

/**
 * Loads field from input
 */
synthesizer_field_tResult getField()
{
    int index;
    input("Zadej cislo polozky: ", "%d", &index);

    if (index < 0 || index >= field_count) {
        return (synthesizer_field_tResult) { .error = 8 };
    }

    return (synthesizer_field_tResult) { fields[index], 0 };
}
