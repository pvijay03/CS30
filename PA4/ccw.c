#include <stdio.h>
#include <stdlib.h>
#include "cw.h"

char buffer[BUFSZ];   // array of characters to loop through
int  cntchar;   // count of chars IN words,not \n, \t or ' '
int  cntword;   // number of words
int  cntline;   // number of \n

int setup(int, char **);   // setup() function declaration
int rd(void);   // rd() function declaration
void result(void);   // result() function declaration

/*
 * function main()
 *
 * usage: ccw [file]
 *
 * Operation: goes through buffers until end
 * of file counting the chars in words, words,
 * and number of new lines
 * 
 * Assumes: any word that is not \n, \t, or ' '
 * is a char in a word
 *
 * Params: argc, argv
 *
 * Returns:  1 (EXIT_FAILURE) if setup does not
 * return 0, other wise 0 (EXIT_SUCCESS);
 *
 */
int
main(int argc, char **argv)
{
    
    /*
     * setup function is called,
     * EXIT_FAILURE if unsuccessful 
     */
    if (setup(argc, argv) != 0)
        return EXIT_FAILURE;

    /*
     * number of chars in buffer
     */    
    int end;

    /*
     * new word: 0 false, 1 true
     */
    int new;

    /*
     * pointer to loop through buffer
     */
    char *ptr;

    /*
     * pointer to terminate looping
     */
    char *endptr;
    
    /*
     * in a loop until end of file
     * call rd() to get a buffer of chars 
     * rd() returns the number of valid chars in the buffer
     * <= 0 signals end of file
     * the buffer contains an array of chars, not a string
     */
    while ((end = rd()) > 0) {
	
	/*
	 * sets ptr to start of buffer
	 */
	ptr = buffer;

        /*
	 * sets endptr to end of buffer
	 */	
	endptr = ptr + end;

	/*
	 * loop through buffer until
	 * the end is reached
	 */
        while (ptr != endptr) {
	    
	    /*
	     * if either space char is found and it
	     * is  newword, increment cntword and new
	     * is now set to 0
	     */
            if (((*ptr == ' ') || (*ptr == '\t'))) {
                ptr++;
		if ( new == 1) {
		    cntword++;
		    new = 0;
		}
	    }

	    /*
	     * if new line char is found, increment
	     * cntline and if it is a new word, increment
	     * cntword. Set new to 0 always
	     */ 
	    else if (*ptr == '\n') {
	        ptr++;
	        cntline++;
	        if (new == 1)
	            cntword++;
	        new = 0;
	    }

	    /*
	     * if a char other than the space chars
	     * and new line char is found, increment
	     * cntchar. A new word is found so set new
	     * to 1
	     */
	    else {
	        ptr++;
	        cntchar++;
	        new = 1;
	    }
        }
    }

    /*
     * edge case: if there is not a new line at the
     * end of the file and there was a new word,	
     *  increment cntword++
     */
    if (new == 1)
        cntword++;
/*
 * add your code to
 * (1)  count the number of characters in the words
 * (not including any spaces, newlines or tabs,
 * (2) the number of words. words are delimited by space, \n or \t
 * (3) the number of \n in the input
 */

    /*
     * result() prints the summary of the values in the global
     * variables above
     * Until you complete result.S you can uncomment #define _PA4DBG_
     * to test your code make sure #define _PA4DBG_ is commented out
     * when you submit your code.
     */
//#define _PA4DBG_
#ifdef _PA4DBG_
     printf("lines:%8.8d words:%8.8d Chars:%8.8d\n", cntline,cntword,cntchar);
#else
    result();
#endif
    return EXIT_SUCCESS;
}
