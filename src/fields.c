#include "fields.h"
#include "types.h"
#include "helpers.h"
#include "comparing.h"
#include<stdio.h>

// --- Fields ---

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
            scanf("%15s", key->name);
            break;
        case MANUFACTURER:
            scanf("%15s", key->manufacturer);
            break;
        case YEAR:
            scanf("%d", &key->year);
            break;
        case VOICES:
            scanf("%d", &key->voices);
            break;
        case ANALOG:
            scanf("%c", &analog);
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
    printf("Zadej cislo polozky: ");
    scanf("%d", &index);

    if (index < 0 || index >= field_count) {
        return (synthesizer_field_tResult) { .error = 8 };
    }

    return (synthesizer_field_tResult) { fields[index], 0 };
}
