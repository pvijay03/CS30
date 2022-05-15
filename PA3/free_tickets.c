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
 * free_tickets
 *      tears down the database freeing all allocated memory
 *
 *      go down each hash chain and free all the vehicles and the tickets
 *      for each vehicle
 *      count the number of tickets freed in the database and
 *      print it out before returning
 *
 *      unsigned long cnt = 0UL
 *      printf("Total tickets freed: %lu\n", cnt);
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */
void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{  
    /* 
     * ticket freed count
     */	
    unsigned long cnt = 0UL;

    /*
     * vehicle pointer to traverse hashtab chain
     */
    struct vehicle *ptr;

    /*
     * vehicle pointer to free vehicles
     */
    struct vehicle *prev;

    /*
     * ticket pointer to traverse vehicle chain
     */
    struct ticket *tptr;

    /*
     * ticket pointer to free tickets
     */
    struct ticket *tprev;

    /*
     * loop to traverse all hashtab chains
     */
    for(uint32_t i = 0; i < tabsz; i++) {
        ptr = *(hashtab + i);
	prev = *(hashtab + i);
	
	/*
	 * loop to traverse all vehicles in a chain and free them
	 * along with the pointers stored within them
	 */
	while(ptr != NULL) {
	    tptr = ptr->head;
	    tprev = ptr->head;

	    /*
	     * loop to traverse all tickets for a vehicle and free them
	     * increments cnt for each freed ticket
	     */
	    while(tptr != NULL) {
	        tprev = tptr;
	        tptr = tptr->next;
	        free(tprev);
	        cnt++;
	    }
	    prev = ptr;
	    ptr = ptr->next;
	    free(prev->state);
	    free(prev->plate);
	    free(prev);
        }
    }

    /*
     * free the empty hash table
     */
    free(hashtab);
    printf("Total tickets freed: %lu\n", cnt);
}
#endif
