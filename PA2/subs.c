#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "subs.h"
#include "misc.h"

/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * args:
 *  buf     one record from read from standind input stream by getline()
 *  delim   the input field delimiter
 *  cnt     number of fields (in table)
 *  table   point to a table of pointers to the start of each record
 *  lineno  line number of record (for passing to dropmesg)
 *  argv    for drop mesg to print name of program
 * returns: 0 if all the columns were found,
 *          -1 if a record did not have all the columns
 *          -1 if a record had an bad field value
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
    int scnt = 0;          /* number of sentinals added  */

    /*
     * processes a line by adding a sentinal at delimiters
     * or new lines and setting a table pointer to the beginning
     * of each field. If there is an invalid field due to quotes
     * return -1, else return 0
     */
    for (int i = 0; i < cnt; i++) {
	
	/*
	 * process field as unquoted
	 * special case: if delim is a quote
	 * all fields should be processed as
	 * unquoted
	 */    
	if ((*buf != '\"') || delim == '\"') {

	    /*
	     * point table pointer at beginning of 
	     * potential field
	     */
	    *(table + i) = buf;

	    /*
	     * searches line until delimeter or new  line is found,
	     * add sentinal, increment buf if delimiter, then break
	     * to go to next table pointer. New line does not step
	     * to the next buf because line is done.
	     */
	    while (*buf != '\0') {

		/* error case: error if quote in unquoted field 
		 * unless delimiter is a quote
		 */
		if ((*buf == '\"') && (delim != '\"')) {
		    dropmsg("A \" is not allowed inside unquoted field",
			    lineno, argv);
		    return -1;
		}
	        if (*buf == delim) {
	            *buf++ = '\0';
		    scnt++;

		    /* speical case: if more sentinals than required
		     * fields, we want to count for later error check
		     */
		    if ((scnt >= cnt) && (*buf != '\0'))
		        continue;
		    break;
	        }
		if (*buf == '\n') {
		    *buf  = '\0';
		    scnt++;
		    break;
		}
	        buf++;
	    }
	}

	/*
	 * process field as quoted
	 */
	else {
	   
           /*
	    * point table pointer at beginning of
	    * potential field
	    */
	   *(table + i) = buf;
	   int qtcnt = 0;           /* quote count */

	   /*
	    * increments buf because there must
	    * be at least one more element in
	    * the field for it to be valid
	    */
	   buf++;

	   /* searches line until delimiter after the quoted field 
	    * or new line ends or new line, add sentinal, increment
	    * buf, then break to go to next table pointer.
	    */
	   while (*buf != '\0') {

	       /* validates additional quotes. Increments qtcnt for 
		* later check, increments buf to validate the next
		* element
		*/
	       if (*buf == '\"') {
	           qtcnt++;
		   buf++;
		   if ((*buf == delim) || (*buf == '\n')) {
		       *buf++ = '\0';
		       scnt++;
		       break;
		   }

		   /*
		    * if buf is poitning to a quote and
		    * qtcnt is even, then the field is valid
		    * to either continue looping or break
		    */
		   if ((*buf == '\"') && (qtcnt%2 == 0)) {
		       if ((*(buf+1) == delim) || (*(buf+1) == delim)) {
		           *(++buf) = '\0';
			   scnt++;
			   buf++;
			   break;
		       }
		       buf++;
		       continue;
		   }
		   /*
		    * if another quote and qtcnt is odd,
		    * keep looping to validate
		    */
		   if (*buf == '\"') {
			buf++;
		        continue;
		   }

		   /*
		    * error case: error if quote after the initial 
		    * quote does not have a quote to the right, entire
		    * line is dropped
		    */
		   dropmsg("Quoted field not terminated properly",
			       lineno, argv);
		   return -1;
	       }
	       else 
	           buf++;
	   }

	   /*
	    * error case: if qtcnt is still 0 after 
	    * finding the initial quote, quoted field was 
	    * never terminated
	    */
	   if (qtcnt == 0) {
	       dropmsg("Quoted field missing final quote",
		       lineno, argv);
	       return -1;
	   }
	}
    }

    /*
     * error case: if more or less fields were
     * found than required, it is an error
     */
    if (scnt > cnt) {
	dropmsg("too many columns", lineno, argv);
	return -1;
    }
    if (scnt < cnt) {
	dropmsg("too few columns", lineno, argv);  
        return -1;
    }	
    return 0; 
}

/*
 * wr_row
 *
 * usage:   given an array of pointers to columns and an array of columns to
 *          ouput and a count of output columns
 *          output those columns specified in the output array (contents are
 *          index numbers of the input array)
 * args:
 *  in_tab  pointer to a table of pointers to the start of each input field
 *  out_tab pointer to a table of ints containing the array index to print
 *          the order in this array is the order the cols/fields are printed
 *  delim   the output record delimiter to use
 *  lineno  line number of record for dropmsg
 *  argv    program name for dropmesg
 * return:  the number of records dropped during output
 *          0 if the record was printed
 *          1 if the record was dropped (special case 1 col and is empty)
 */
int
wr_row(char **in_tab, int *out_tab, int out_cnt, char delim,
        unsigned long lineno, char **argv)
{   
    /*
     * error case: if there is only one output column
     * and the in_tab pointer is pointing at an empty field,
     * it is an error
     */
    if ((out_cnt == 1) && **(in_tab + *out_tab) == '\0') {
        dropmsg("empty column", lineno, argv);
	return 1;
    }

    /*
     * loops through the number of out columns and
     * prints what the in_tab pointer is pointing at
     * with a deilimiter or new line if it is the last
     * column
     */
    for (int i = 0; i < out_cnt; i++)
    {
	if(i <  (out_cnt - 1)) {
            printf("%s%c", *(in_tab + *out_tab), delim);
        }
	else {
	    printf("%s\n", *(in_tab + *out_tab));
	}
	out_tab++;
    }
    return 0;
}
