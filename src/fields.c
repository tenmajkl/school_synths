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
    { NAME, "jmena", "jmeno", "%15s", byNameCondition },
    { MANUFACTURER, "vyrobce", "vyrobce", "%15s", byManufacturerCondition },
    { YEAR, "roku vydani", "rok vydani", "%d", byYearCondition },
    { VOICES, "poctu hlasu", "pocet hlasu", "%d", byVoicesCondition },
    { ANALOG, "analogovosti", "analogovy", "%d", byAnalogCondition }
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
    puts("Upravit polozku:");
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
    char prompt[32];
    sprintf(prompt, "Zadej %s: ", field.edit_description);
    clearBuffer();
    char analog;
    
    switch (field.index) {
        case NAME:
            input(prompt, "%15[\n]", key->name);
            break;
        case MANUFACTURER:
            input(prompt, "%15[\n]", key->manufacturer);
            break;
        case YEAR:
            input(prompt, "%d", &key->year);
            break;
        case VOICES:
            input(prompt, "%d", &key->voices);
            break;
        case ANALOG:
            input(prompt, "%c", &analog);
            key->analog = analog == 'y';
            break;
    }

}

/**
 * Loads field from input
 */
synthesizer_field_result_t getField()
{
    int index;

    do {
        input("Zadej cislo polozky: ", "%d", &index);
    } while (index < 0 || index >= field_count);

    return (synthesizer_field_result_t) { fields[index], 0 };
}
