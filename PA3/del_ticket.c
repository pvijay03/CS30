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
 * del_ticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the strcut vehicle.      
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid, argv) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d 
 *
 * Inputs:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine   
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns 0 if ok -1 if not found or error
 */
int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
	/*
	 * vehicle pointer to traverse chain
	 */
	struct vehicle *ptr;

	/*
	 * vehicle pointer to track previous
	 * vehicle for deletion if no tickets
	 */
	struct vehicle *vprev;

	/*
	 * index of chain
	 */
	uint32_t index = hash(plate, argv) % tabsz;

	/*
	 * variable to store summons
	 */
	unsigned long long summid;

	/*
	 * ticket pointer to traverse tickets
	 * for a vehicle
	 */
	struct ticket *tptr;

	/*
	 * ticket pointer to track previous
	 * ticket for deletion
	 */
	struct ticket *prev;

	/*
	 * checks if index is within table
	 */
	if (index > (tabsz-1)) {
	    fprintf(stderr, "Invalid Index");
            return -1;
	}

	/*
	 * coverts string to summid
	 */
	if (strtosumid(summ, &summid, argv) != 0) {
	    fprintf(stderr, "strtosumid Error");
	    return -1;
	}

	/*
	 * sets vehicle pointers to beginning
	 * of chain
	 */
	ptr = *(hashtab + index);
	vprev = *(hashtab + index);

	/*
	 * error if chain is empty
	 */
	if (ptr == NULL) {
	    free(ptr);
	    fprintf(stderr, "Empty Chain");
	    return -1;
	}

	/*
	 * loop through chain to find vehicle
	 * with potential ticket
	 */
	while (ptr != NULL) {
	    if (((strcmp(ptr->plate, plate)) == 0) &&
	        ((strcmp(ptr->state, state)) == 0))
	         break;
	    vprev = ptr;
	    ptr = ptr->next;
	}

	/*
	 * error if vehicle not found
	 */
	if (ptr == NULL) {
	    fprintf(stderr, "Vehicle Not Found");
	    return -1;
	}

	/*
	 * set ticket pointers to beginning
	 * of ticekt chain for vehicle
	 */
	tptr = ptr->head;
	prev = ptr->head;

	/*
	 * loop through tickets to find
	 * potential ticket
	 */
	while (tptr != NULL) {

	    /*
	     * if found, update vehicles ticket
	     * count and total fine
	     */
	    if (tptr->summons == summid) {
		ptr->cnt_ticket = ptr->cnt_ticket - 1;
		ptr->tot_fine = ptr->tot_fine - ((fineTab + (tptr->code))->fine);
                break;	
	    }
	    prev = tptr;
	    tptr = tptr->next;
	}

	/*
	 * error if ticket not found
	 */
	if (tptr == NULL) {
	    fprintf(stderr, "Ticket Not Found");
	    return -1;
	}

	/*
	 * if ticket was the first ticket in the
	 * chain, update the ticket pointer in vehicle.
	 * Else the previous ticket pointer now contains
	 * the ticket pointer to the next ticket
	 */
	if (tptr == (ptr->head))
	    ptr->head = tptr->next;
	else
	    prev->next = tptr->next;

	/*
	 * if vehicle has no more tickets, adjust pointers in chain
	 * and delete the vehicle by freeing up its malloc'ed space
	 */
	if (ptr->cnt_ticket == 0) {
	    if (vprev->next == ptr->next)
		*(hashtab + index) = ptr->next;
	    else 
		vprev->next = ptr->next;
	    free(ptr->state);
	    free(ptr->plate);
	    free(ptr);
	}

	/*
	 * delete the ticket by freeing up its malloc'ed space
	 */
	free(tptr);
	return 0;
}
#endif
