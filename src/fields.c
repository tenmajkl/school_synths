/**
 * Synths - fields
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

#include "fields.h"
#include "types.h"
#include "helpers.h"
#include "comparing.h"
#include<stdio.h>

// --- Fields ---

/**
 * Definitions of fields which can be used for filtering, editing and other stuff
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
 * Outputs menu for filtering fields
 */
void fieldsFilterMenu(void)
{
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index + 1, fields[index].filter_description);
    }
    puts("Pro ukonceni napis 0");
}

/**
 * Outputs menu for editing fields
 */
void fieldsEditMenu(void)
{
    puts("Upravit polozku:");
    for (int index = 0; index < field_count; index++) {
        printf("%d. %s\n", index + 1, fields[index].edit_description);
    }
    puts("Pro ukonceni napis 0");
}

/**
 * loads value to given synthesizer to given field represented by synthesizer_field_t
 *
 * @param synthesizer_field_t field field which value will be loaded
 * @param synthesizer_t* key synthesizer where value will be stored
 */
void getKeyByField(synthesizer_field_t field, synthesizer_t* key)
{
    char prompt[32];
    sprintf(prompt, "Zadej %s: ", field.edit_description);
    clearBuffer();
    char analog;
    
    switch (field.index) {
        case NAME:
            input(prompt, "%15[^\n]", key->name);
            break;
        case MANUFACTURER:
            input(prompt, "%15[^\n]", key->manufacturer);
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
 * Returns user selected field
 *
 * @return synthesizer_field_result_t field with error code 0 or 9
 */
synthesizer_field_result_t getField(void)
{
    int index;

    do {
        input("Zadej cislo polozky: ", "%d", &index);
        if (index == 0) {
            return (synthesizer_field_result_t) { .error = 9 };
        }
    } while (index < 1 || index > field_count);

    return (synthesizer_field_result_t) { fields[index - 1], 0 };
}
