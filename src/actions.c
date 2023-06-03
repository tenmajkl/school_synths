/**
 * Synths - actions
 *
 * @author Michal Kripac
 * @year 2023
 * @license GPL3 
 */

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
 * Asks for item description and ats it to given list. When list capacity is full, it reallocates the list with bigger capacity
 *
 * @param synthesizer_array_t* list list where item will be added
 * @return int error code 1 or 0
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

    input("Zadej jmeno modelu: ", "%15[^\n]", item.name);

    input("Zadej jmeno vyrobce: ", "%15[^\n]", item.manufacturer);

    input("Zadej rok vydani: ", "%d", &item.year);

    input("Zadej pocet hlasu: ", "%d", &item.voices);

    clearBuffer();
    char analog;
    input("Je analogovy? y/n: ", "%c", &analog);
    item.analog = analog == 'y';

    item.deleted = false;

    item.id = list->size > 1 ? list->array[list->size - 2].id + 1 : 1;

    list->array[list->size - 1] = item;
    list->indexes.array[list->indexes.size - 1] = list->size - 1;

    return 0;
}

/**
 * Asks for field, its value and then prints all items where field has given value
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be used for filtration
 * @return int error code 0, 2 or 3 
 */
int filterDialogueAction(synthesizer_array_t* list)
{ 
    puts("Filtrovat podle:");
    synthesizer_array_result_t models = filterWithDialogue(list);
    
    if (models.error == 9) {
        return 0;
    }

    if (models.error != 0) {
        return models.error;
    }
    
    print(&models.result);

    free(models.result.indexes.array);

    return 0;
}

/**
 * Asks for field used for sorting and uses it to sort given list using sortDialogue
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be sorted
 * @return int error code 0, 5 or 3 
 */
int sortAction(synthesizer_array_t* list) 
{
    puts("Seradit podle:");
    fieldsFilterMenu();
    synthesizer_field_result_t field = getField();

    if (field.error == 9) {
        return 0;
    }

    if (field.error != 0) {
        return field.error;
    }

    int choice;
    input("Chces data jen vypsat (1), nebo i ulozit (2)? Pozn. pouze vypsani zabere vice pameti. ", 
          "%i", 
          &choice
    );
    if (choice == 1) {
        synthesizer_array_result_t copied_r = copy(*list);

        if (copied_r.error != 0) {
            return copied_r.error;
        }

        synthesizer_array_t copied = copied_r.result;

        int code;
        if ((code = sort(&copied, field.result.condition)) != 0) {
            return code;
        }
        print(&copied);
        free(copied.indexes.array);
        return 0;
    }

    return sort(list, field.result.condition);
}

/**
 * Using filterWithDialogue finds items which will then be edited by user.
 *     Every item is printed and user can choose which fields to edit separately.
 *
 * @see fields.c for editing fields
 * @see lists.c for filtering
 * @param synthesizer_array_t* list list which will be edited 
 * @return int error code 0, 2 or 3 
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

        printf("Uprava nasledujici polozky:\n");
        printHead();
        writeOne(stdout, PRETTY_FORMAT, *item);
        fieldsEditMenu();
        
        synthesizer_field_result_t field = getField(); 
        while (field.error != 9) {
            if (field.error != 0) {
                return field.error;
            }

            getKeyByField(field.result, item);
            field = getField();
        }
    }

    return 0;
}

/**
 * Using filterWithDialogue finds items which will then be deleted by user.
 *     Every item is printed and user can choose whenever they want to delete it.
 *
 * @see lists.c for filtering
 * @param synthesizer_array_t* list list which will be deleted from
 * @return int error code 0, 2 or 3
 */
int deleteAction(synthesizer_array_t *list)
{
    puts("Vyhledat podle:");
    synthesizer_array_result_t result = filterWithDialogue(list);
    if (result.error != 0) {
        return result.error;
    }

    synthesizer_array_t array = result.result;
    synthesizer_t *item;
    char anwser;

    for (int i = 0; i < array.indexes.size; i++) {
        item = get(&array, i).result;
        printf("Chces smazat nasledujici polozku? (y/n)\n");
        printHead();
        writeOne(stdout, PRETTY_FORMAT, *item);
        input("> ", "%c", &anwser);
        if (anwser == 'y') {
            item->deleted = true;
        }
    }

    return 0;
}

/**
 * Prints oldest item in given list
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be used for finding oldest item
 * @return int error code 0 or 2
 */
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
