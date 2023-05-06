#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "types.h"

/**
 * Adds item to list
 */
int addItemAction(synthesizer_array_t* list);

/**
 * Prints models from loaded manufacturer
 */
int filterDialogueAction(synthesizer_array_t* list);


int sortAction(synthesizer_array_t* list);

/**
 * Searches for given item
 */
int searchDialogue(synthesizer_array_t *list);

/**
 * Edits given item
 */
int editAction(synthesizer_array_t* list);


/**
 * Removes given item
 */
int deleteAction(synthesizer_array_t *list);


/**
 * Outputs oldest item
 */
int oldestAction(synthesizer_array_t* list);

#endif 
