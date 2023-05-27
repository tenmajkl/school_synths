#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "types.h"

/**
 * Asks for item description and ats it to given list. When list capacity is full, it reallocates the list with bigger capacity
 *
 * @param synthesizer_array_t* list list where item will be added
 * @return int error code 1 or 0
 */
int addItemAction(synthesizer_array_t* list);

/**
 * Asks for field, its value and then prints all items where field has given value
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be used for filtration
 * @return int error code 0, 2 or 3 
 */
int filterDialogueAction(synthesizer_array_t* list);

/**
 * Asks for field used for sorting and uses it to sort given list using sortDialogue
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be sorted
 * @return int error code 0, 5 or 3 
 */
int sortAction(synthesizer_array_t* list);

/**
 * Using filterWithDialogue finds items which will then be edited by user.
 *     Every item is printed and user can choose which fields to edit separately.
 *
 * @see fields.c for editing fields
 * @see lists.c for filtering
 * @param synthesizer_array_t* list list which will be edited 
 * @return int error code 0, 2 or 3 
 */
int editAction(synthesizer_array_t* list);


/**
 * Using filterWithDialogue finds items which will then be deleted by user.
 *     Every item is printed and user can choose whenever they want to delete it.
 *
 * @see lists.c for filtering
 * @param synthesizer_array_t* list list which will be deleted from
 * @return int error code 0, 2 or 3
 */
int deleteAction(synthesizer_array_t *list);


/**
 * Prints oldest item in given list
 *
 * @see lists.c
 * @param synthesizer_array_t* list list which will be used for finding oldest item
 * @return int error code 0 or 2
 */
int oldestAction(synthesizer_array_t* list);

#endif 
