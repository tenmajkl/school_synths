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
 * loads value to given synthesizer to given field represented by synthesizer_field_t
 *
 * @param synthesizer_field_t field field which value will be loaded
 * @param synthesizer_t* key synthesizer where value will be stored
 */
void getKeyByField(synthesizer_field_t field, synthesizer_t* key);

/**
 * Returns user selected field
 *
 * @return synthesizer_field_result_t field with error code 0 or 9
 */
synthesizer_field_result_t getField();

#endif
