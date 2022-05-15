/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"

int 
main(void)
{
    int c; // Each character input
    int cnt = 0; // C sytle comment new lines
    int lncnt = 1; // Line number
    enum typestate state = START;
    while ((c = getchar()) != EOF) {
	switch (state) {
        case START: // nothing
            state = STARTst(c);
            if (c != '/')
                putchar(c);
	    if (c == '\n') {
		lncnt++;
	    }
	    break;
        case SLASH: // saw a /
            state = SLASHst(c);
            if ((c != '/') && (c != '*')) {
                putchar('/');
                putchar(c);
            }
            break;
        case SLASHSLASH: // saw a //
            state = SLASHSLASHst(c);
            if (c == '\n') {		
                putchar(' ');
		putchar('\n');
            }
            break;
        case SLASHSTAR: // saw a /*
            state = SLASHSTARst(c);
            if (c == '\n') {
                cnt++;
	    }
            break;
        case CLOSESTAR: // saw a *
            state = CLOSESTARst(c);
	    if (c == '\n')
		cnt++;
            if (c == '/') {
                putchar(' ');
                for(int i = 0; i < cnt; i++)
                    putchar('\n');
            }
            break;
        case STRLITBEG: // saw a " 
            state = STRLITBEGst(c);
            if (c != '\\')
                putchar(c);
            break;
        case STRESCAPE: // saw a backslash 
            state = STRESCAPEst();
            if (c != '\\') {
		putchar('\\');
                putchar(c);
	    }
	    break;
        case CHARLITBEG: // saw a ' 
            state = CHARLITBEGst(c);
            if (c != '\\')
                putchar(c);
            break;
        case CHARESCAPE: // saw a backslash
            state = CHARESCAPEst();
            if ( c != '\\') {
	        putchar('\\');
                putchar(c);
	    }
            break;
	default: // state was not handled
	    fprintf(stderr, "Program error, DFA state not handled\n");
	    return EXIT_FAILURE;
	    break;
        }
    }
    /*
     * Special case of if there is an EOF afer a 
     * single /, then it needs to putchar('/') here
     * since the while loop terminates without printing
     * that single /
     */
    if (state == SLASH)
        putchar('/');
    /*
     * Special case of if there is an EOF before a C
     * style comment ends. This ensures that a blank
     * space followed by the number of new lines after
     * the start of the comment is printed for standard
     * output
     */
    if ((state == SLASHSTAR) || (state == CLOSESTAR)) {
	putchar(' ');
        for (int i = 0; i < cnt; i++) {
	    putchar('\n');
	}
    }
    /* 
     * Handles standard error output 
     */
    switch (state) {
    case START:
    	return EXIT_SUCCESS;
    case SLASH:
	return EXIT_SUCCESS;
    case SLASHSLASH:
	return EXIT_SUCCESS;	
    case SLASHSTAR:
        fprintf(stderr, "Error: line %i: unterminated comment\n",lncnt);
        return EXIT_FAILURE;
    case CLOSESTAR:
        fprintf(stderr, "Error: line %i: unterminated comment\n",lncnt);
        return EXIT_FAILURE;
    case STRLITBEG:
        fprintf(stderr, "Error: line %i: unterminated quote(\")\n",lncnt);
        return EXIT_FAILURE;
    case STRESCAPE:
        fprintf(stderr, "Error: line %i: unterminated quote(\")\n",lncnt);
        return EXIT_FAILURE;
    case CHARLITBEG:
        fprintf(stderr, "Error: line %i: unterminated quote(\')\n",lncnt);
        return EXIT_FAILURE;
    case CHARESCAPE:
        fprintf(stderr, "Error: line %i: unterminated quote(\')\n",lncnt);
        return EXIT_FAILURE;
    default: // state was not handled
	fprintf(stderr, "Program error, DFA state not handled\n");
	return EXIT_FAILURE;
    }
}
