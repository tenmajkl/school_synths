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
    if (list->size == list->capacity) {
        synthesizer_t* new;
        int* new_indexes;
        new = realloc(list->array, list->size);
        if (new == NULL) {
            return 1;
        }

        new_indexes = realloc(list->indexes.array, list->size);
        if (new_indexes == NULL) {
            free(new);
            return 1;
        }

        list->capacity++;
        list->indexes.capacity++;

        list->indexes.array = new_indexes;
        list->array = new;
    }
    list->size++;
    list->indexes.size++;

    synthesizer_t item;
    int result = loadOne(stdin, &item, USER_INPUT_FORMAT, false, 'y');

    item.id = list->size > 1 ? list->array[list->size - 2].id + 1 : 1;

    list->array[list->size - 1] = item;
    list->indexes.array[list->indexes.size - 1] = list->size - 1;

    return 0;
}

/**
 * Prints models from loaded manufacturer
 */
int filterDialogueAction(synthesizer_array_t* list)
{ 
    puts("Filtrovat podle:");
    synthesizer_array_result_t models = filterWithDialogue(list);
    if (models.error != 0) {
        return models.error;
    }
    
    printHead();
    write(stdout, models.result, PRETTY_FORMAT);

    free(models.result.indexes.array);

    return 0;
}

int sortAction(synthesizer_array_t* list) 
{
    puts("Seradit podle:");
    fieldsFilterMenu();
    synthesizer_field_result_t field = getField();

    if (field.error != 0) {
        return field.error;
    }

    return sortDialogue(list, field.result.condition);
}

/**
 * Edits given item
 */
int editAction(synthesizer_array_t* list)
{
    puts("Vyhledat podle:");
    synthesizer_array_result_t result = filterWithDialogue(list);

    if (result.error != 0) {
        return result.error;
    }

    synthesizer_array_t array = result.result;

    synthesizer_t *item;

    for (int i = 0; i < array.indexes.size; i++) {
        item = get(&array, i).result;

        printf("-- Uprava polozky s id %i --\n", item->id);
        fieldsEditMenu();
        
        synthesizer_field_result_t field = getField(); 
        if (field.error != 0) {
            return field.error;
        }

        getKeyByField(*list, field.result, item);
    }

    return 0;
}

/**
 * Removes filtered items
 */
int deleteAction(synthesizer_array_t *list)
{
    puts("Vyhledat podle:");
    synthesizer_array_result_t result = filterWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    synthesizer_array_t array = result.result;

    for (int i = 0; i < array.indexes.size; i++) {
        get(&array, i).result->deleted = true;
    }

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
