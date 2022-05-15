/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"
/*
 * function STARTst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: SLASH if / was found, STRLITBEG
 * if \" was found, CHARLITBEG is \' was 
 * found, START otherwise
 */
enum typestate
STARTst(int c)
{
    if (c == '/')
        return SLASH;
    else if (c == '\"')
        return STRLITBEG;
    else if (c == '\'')
        return CHARLITBEG;
    return START;
}
/*
 * function SLASHst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: SLASHSTAR if * was found, SLASHSLASH
 * if / was found, START otherwise
 */
enum typestate
SLASHst(int c)
{
    if (c == '*')
        return SLASHSTAR;
    else if (c == '/')
        return SLASHSLASH;
    return START;
}
/*
 * function SLASHSLASHst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: START if \n was found, SLASHSLASH
 * otherwise
 */
enum typestate
SLASHSLASHst(int c)
{
    if (c == '\n')
        return START;
    return SLASHSLASH;
}
/*
 * function SLASHSTARst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: CLOSESTAR if * was found, otherwise
 * SLASHSTAR
 */
enum typestate
SLASHSTARst(int c)
{
    if (c == '*')
        return CLOSESTAR;
    return SLASHSTAR;
}
/*
 * function CLOSESTARst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: START is / was found, CLOSESTAR if *
 * was found, SLASHSTAR otherwise
 * 
 */
enum typestate
CLOSESTARst(int c)
{
    if (c == '/')
        return START;
    else if (c == '*')
        return CLOSESTAR;
    return SLASHSTAR;
}
/*
 * function STRLITBEGst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 * params: c
 * returns: STRESCAPE if \\ was found, START
 * if \" was found, STRLITBEG otherwise
 */
enum typestate
STRLITBEGst(int c)
{
    if (c == '\\')
        return STRESCAPE;
    else if (c == '\"')
        return START;
    return STRLITBEG;
}
/*
 * function STRESCAPEst()
 *
 * Operation: Handles escaped double quotes
 * params: none
 * returns: STRLITBEG
 */
enum typestate
STRESCAPEst()
{
    return STRLITBEG;
}
/*
 * function CHARLITBEGst()
 *
 * Operation: takes a character and changes
 * the state depending on what the character
 * is
 *
 * params: c
 * returns: CHARESCAPE if \\ was found, START
 * if \' was found, CHARLITBEG otherwise
 */
enum typestate
CHARLITBEGst(int c)
{
    if (c == '\\')
        return CHARESCAPE;
    else if (c == '\'')
        return START;
    return CHARLITBEG;
}
/*
 * function CHARESCAPE()
 *
 * Operation: Handles escaped single quotes
 * params: none
 * returns: CHARLITBEG
 */
enum typestate
CHARESCAPEst()
{
    return CHARLITBEG;
}
