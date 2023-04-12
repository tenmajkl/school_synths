#ifndef FIELDS_H_
#define FIELDS_H_

#include "types.h"
#include "comparing.h"

/**
 * Fields which can be used for editing or grouping
 */
extern const int field_count;
extern const synthesizer_field_t fields[];

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
