#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "list.h"
#include "malloc.h"

/*
 * author Nick Wilson and Kao Thao
 * version 3.14.13
 *
 * Consists of three functions one that initializes a list and one that adds a
 * item to the list, and one that frees all elements in a list
 */

/** list_init() - function is used to create a new list, it does this with 
 * three main steps.
 * 1 - malloc() a new list_t structure.
 * 2 - set the head and tail elements of the structure to NULL
 * 3 - return a pointer to the list_t to the caller.
 *
 * Returns: list_t - a list that has been malloced and is empty
 */
list_t *list_init(void) {

    // call our malloc function i guess the size of a struct
    list_t *node_ptr = (list_t *)Malloc(sizeof(list_t));
    if (node_ptr == NULL) {
        exit(EXIT_FAILURE);
    }

    node_ptr->first = NULL;
    node_ptr->last = NULL;
    return node_ptr;
}

/** list_add() - function adds the object structure pointed to by new to the
 * list structure pointed to by the list. Two cases must be distinguished
 * 1 - the list is empty (list->first == NULL)
 * 2 - the list is not empty
 *
 * Parameters:
 * list - a list, it is of type list_t
 * new  - the new object that will be added to the list, type obj_t
 */
void list_add(list_t *list, obj_t *new) {
    if (list->first == NULL) {
        list->first = new;
        list->last = new;
        new->next = NULL;
    } else {
        (list->last)->next = new;
        new->next = NULL;
        list->last = new;
    }
}

/*
 * free_list - deletes the contents of the list
 * This function will make a temp "node" that points to the head of the list.
 * Then it makes "head" point to the next "node".
 * then it deletes the node that the temp node is pointing to, and continues
 * this until all nodes in list are deleted.
 * Parameter:
 * list - a list whose contents must be freed
 */



////////////// still leaking in here on spotlights at least///////////
void free_list(list_t *list) {
    while (list->first) {
        
        obj_t *victim = (obj_t *) list->first;
        list->first = list->first->next;
        if (victim->priv != NULL) {
            free_inside(victim->priv);
        }
       free(victim);
       //list->first = temp;
    }

    free(list);
}

// recursively traverse levels to free objects
void free_inside(obj_t *object) {
    
    if (object->objtype != 10 || object->objtype != 14 || object->objtype != 15) { // ||
       // object->objtype != 16) {
        
        free(object);
    } else {
        obj_t *temp = (obj_t *) object->priv;
        free_inside(temp);
    }
}
