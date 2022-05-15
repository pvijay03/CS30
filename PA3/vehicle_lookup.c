#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * vehicle_lookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  argv    for error printing
 *
 * returns  pointer to vehicle if found NULL otherwise
 */
struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
    char *state, char **argv)
{
    /*
     * chain index for vehicle
     */
    uint32_t index = hash(plate, argv) % tabsz;

    /*
     * checks if index is within table
     */
    if (index > (tabsz - 1)) {
    	fprintf(stderr, "%p: Invalid Index - %d", *argv, index);
	return NULL;
    }

    /*
     * vehicle pointer to traverse hashtab chain
     */
    struct vehicle *ptr = *(hashtab + index);

    /*
     * traverses chain, if vehicle found with ptr, break
     */
    while (ptr != NULL) {
        if (((strcmp(ptr->plate, plate)) == 0 ) &&
            ((strcmp(ptr->state, state)) == 0))
	    break;
	ptr = ptr->next;
    }
    return ptr;
}
#endif
