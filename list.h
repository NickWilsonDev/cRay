#ifndef LIST_H
#define LIST_H

#include "structs.h"

/*
 * author Nick Wilson and Kao Thao
 * version 3.20.13
 * list.h - file contains prototypes for the functions contained in the list.c
 *          file.
 */

/*
 * list_init() - function prototype, function is used to initialize a list
 *
 * Returns: a list_t struct
 */
list_t *list_init(void);

/*
 * list_add() - prototype, function adds an object to the list specified in 
 *              the parameter
 *
 * Parameters: 
 *          list - type list_t, that we will be adding an object to
 *          new  - type obj_t, that will be added to the list
 */
void list_add(list_t *list, obj_t *new);

/*
 * free_list() - prototype, function traverses the list and frees the data 
 *               stored in the "node", then frees the node and repeates this 
 *               until list is freed
 * Parameters:
 *          list - type list_t, the list that we wish to free
 */
void free_list(list_t *list);

void free_inside(obj_t *object);
#endif
