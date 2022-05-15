/* 
 * the next two lines are the header guards
 */
#ifndef _OMIT_H
#define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 * function prototypes
 */
/*
 * Each DFA state
 */
enum typestate {START, SLASH, SLASHSLASH, SLASHSTAR, CLOSESTAR, STRLITBEG,
   	        STRESCAPE, CHARLITBEG, CHARESCAPE}; 
/*
 * START state function
 */
enum typestate STARTst(int); 
/*
 * SLASH state function
 */
enum typestate SLASHst(int);
/*
 * SLASHSLASH state function
 */
enum typestate SLASHSLASHst(int);
/*
 * SLASHSTAR state function
 */
enum typestate SLASHSTARst(int);
/*
 * CLOSESTAR state function
 */
enum typestate CLOSESTARst(int);
/*
 * STRLITBEG state function
 */
enum typestate STRLITBEGst(int);
/*
 * STRESCAPE state function
 */
enum typestate STRESCAPEst();
/*
 * CHARLITBEG state function
 */
enum typestate CHARLITBEGst(int);
/*
 * CHARESCAPE state function
 */
enum typestate CHARESCAPEst();
#endif
