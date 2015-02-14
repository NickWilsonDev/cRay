#include <stdio.h>
#include <stddef.h> //i added this should contain size_t def
#include <stdlib.h>
#include "malloc.h"

/*
 * Malloc.c - File contains function that takes the place of the standard
 *            malloc function. Function does some error checking as well.
 */
void* Malloc(size_t size){
    void* mallocPtr;
    mallocPtr = malloc(size);
    if(!mallocPtr){
        printf("Not enough memory to be allocated");
        exit(1);
    }
    return mallocPtr;
}
