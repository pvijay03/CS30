#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * insert_ticket
 *      add a ticket (summons) to the database
 *
 *      YOU CANNOT USE CALLOC(), only malloc() is allowed.
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *          example hashval = hash(plate, argv) % tabsz;
 *  fineTab pointer fine table. maps code number to text description & fine cost 
 *          you use this table to get the amount of the fine when calculating
 *          the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *  summ    summons id string to be added
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string         
 *          Conversion function is supplied. use example:
 *               long long summid;
 *               if (strtosumid(summ, &summid, argv) != 0)
 *                  error handling
 *               new_ticket->summons = summid;
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          The encoding into a number uses Linux time format.
 *          Conversion function is supplied. use example:
 *              time_t dateval;
 *              if (strtoDate(date, &dateval, argv) != 0)
 *                  error handling
 *              new_ticket->date = dateval;
 *  code    summons code integer value used an an index into the fines table
 *  argv    for error printing
 *
 * returns 0 if ok -1 for all errors
 */
int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab, char *summ,
            char *plate, char *state, char *date, int code, char **argv)
{   
    /*
     * variable to store the summons
     */
    unsigned long long summid;

    /*
     * variable to store the date
     */
    time_t dateval;

    /*
     * index of chain to insert vehicle and/or ticket
     */
    u_int32_t index = hash(plate, argv) % tabsz;

    /* 
     * convert string to the summid with error check
     */
    if (strtosumid(summ, &summid, argv) != 0) {
        fprintf(stderr, "strtosumid Error");
	return -1;
    }

    /*
     * convert string to dateval with error check
     */
    if (strtoDate(date, &dateval, argv) != 0) {
	fprintf(stderr, "stroDate Error");
	return -1;
    }

    /*
     * checks if index is within the table
     */
    if (index > (tabsz - 1)) {
        fprintf(stderr, "%p: Invalid Index - %d", *argv, index);
	return -1;
    }

    /*
     * vehicle pointer to traverse chain
     */    
    struct vehicle *ptr = *(hashtab + index);

    /*
     * loop through the chain to find the vehicle
     */
    while (ptr != NULL) {
	if (((strcmp(ptr->plate, plate)) == 0 ) &&
            ((strcmp(ptr->state, state)) == 0))
	    break;    
        ptr = ptr->next;
    }

    /*
     * if the vehicle does not exist in the chain, add it
     */
    if (ptr == NULL) {

	/*
	 * malloc space for the new vehicle
	 */
        struct vehicle *new = malloc(sizeof(*new));

	/*
	 * if malloc successful, space for two strings
	 * are malloc'ed and vehicle fields are filled.
	 * Else, error
	 */
	if(new != NULL) {
	    if ((new->state = strdup(state)) == NULL) {
		fprintf(stderr, "State Allocation Error");
	        return -1;
	    }
	    if ((new->plate = strdup(plate)) == NULL) {
		fprintf(stderr, "Plate Allocation Error");
		return -1;
	    }
	    new->tot_fine = 0;
	    new->cnt_ticket = 0;
	    new->next = *(hashtab + index);
	    new->head = NULL;
	} else {
	    fprintf(stderr, "Vehicle Allocation Error");
	    return -1;
        }

	/*
	 * inserts vehicle at head of the chain
	 */
        *(hashtab + index) = new;
	ptr = new;
    }

    /*
     * malloc space for the new ticket
     */
    struct ticket *tnew = malloc(sizeof(*tnew));

    /*
     * if malloc successful, ticket fields are
     * filled. Else, error
     */
    if (tnew != NULL) {
	tnew->summons = summid;
	tnew->date = dateval;
	tnew->code = code;
	tnew->next = NULL;
    } else {
	fprintf(stderr, "Ticket Allocation Error");
        return -1;
    }
    /*
     * ticket pointer to traverse vehicle's tickets
     */
    struct ticket *tptr = ptr->head;
    
    /*
     * ticket pointer to add the new ticket
     */
    struct ticket *tprev = ptr->head;
    /*
     * traverse ticket chain to the end of the list
     */
    while (tptr != NULL) {
	
	/*
	 * if there is a ticket with the same summons, error
	 */
	if (tptr->summons == summid) {
	    fprintf(stderr, "Summons: %llu already exists\n", summid);
	    return -1;
	}
	tprev = tptr;
        tptr = tptr->next;
    }

    /*
     * if no tickets, a vehicle field points to the new ticket,
     * else, the last ticket points to the new ticket
     */
    if (tprev == NULL) {
        ptr->head = tnew;
    } else {
        tprev->next = tnew;
    }

    /*
     * updates the count of tickets and the total fine
     * for the vehicle
     */
    ptr->cnt_ticket = ptr->cnt_ticket + 1;
    ptr->tot_fine = ptr->tot_fine + ((fineTab + code)->fine);
    return 0;   
}
#endif
