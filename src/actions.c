// --- Actions from menu ---
#include "actions.h"
#include "lists.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>

#include "helpers.h"
#include "fields.h"
#include "comparing.h"
#include "types.h"


/**
 * Adds item to list
 */
int addItemAction(synthesizer_array_t* list)
{
    synthesizer_t* new;
    if (list->size == list->capacity) {
        list->capacity++;
        new = realloc(list->array, list->size);
        if (new == NULL) {
            return 1;
        }
        list->array = new;
    }
    list->size++;

    synthesizer_t item;

    input("Zadej jmeno modelu: ", "%15s", item.name);

    input("Zadej jmeno vyrobce: ", "%15s", item.manufacturer);

    input("Zadej rok vydani: ", "%d", &item.year);

    input("Zadej pocet hlasu: ", "%d", &item.voices);

    clearBuffer();
    char analog;
    input("Je analogovy? y/n: ", "%c", &analog);
    item.analog = analog == 'y';

    item.deleted = false;

    item.id = list->size > 1 ? list->array[list->size - 2].id + 1 : 1;

    list->array[list->size - 1] = item;
    return 0;
}

/**
 * Prints models from loaded manufacturer
 */
int filterDialogueAction(synthesizer_array_t* list)
{ 
    fieldsFilterMenu();
    synthesizer_field_result_t field = getField();
    if (field.error != 0) {
        return field.error;
    }
    synthesizer_t key;
    getKeyByField(*list, field.result, &key);

    synthesizer_array_result_t models = filter(*list, key, field.result.condition);
    if (models.error != 0) {
        return models.error;
    }
    
    printHead();
    write(stdout, models.result, PRETTY_FORMAT);

    free(models.result.indexes.array);

    return 0;
}

/**
 * Menu item for sorting by year
 */
int sortByYearAction(synthesizer_array_t* list)
{
    return sortDialogue(list, byYearCondition);
}

/**
 * Menu item for sorting by name
 */
int sortByNameAction(synthesizer_array_t* list)
{
    return sortDialogue(list, byNameCondition);
}

/**
 * Searches for given item
 */
int searchDialogue(synthesizer_array_t *list)
{
    synthesizer_result_t result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *result.result);
    return 0;
}

/**
 * Edits given item
 */
int editAction(synthesizer_array_t* list)
{
    synthesizer_result_t result = searchWithDialogue(list);

    if (result.error != 0) {
        return result.error;
    }

    synthesizer_t* item = result.result;

    fieldsEditMenu();
    synthesizer_field_result_t field = getField(); 
    if (field.error != 0) {
        return field.error;
    }
    getKeyByField(*list, field.result, item);

    return 0;
}

/**
 * Removes given item
 */
int deleteAction(synthesizer_array_t *list)
{
    synthesizer_result_t result = searchWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    result.result->deleted = true;

    return 0;
}

// Outputs oldest item
int oldestAction(synthesizer_array_t* list)
{
    synthesizer_result_t result = getOldest(*list);
    
    if (result.error != 0) {
        return result.error;
    }

    printHead();
    writeOne(stdout, PRETTY_FORMAT, *(result.result));
    return 0;
}
