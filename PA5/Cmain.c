#include <stdio.h>
#include <stdlib.h>
#include "encrypter.h"
#define BUFSZ   1024

int
main(int argc, char **argv)
{
    FILE *FPBOOK;
    FILE *FPIN;
    FILE *FPOUT;
    int MODE;
    char IOBUF[BUFSZ];
    char BOOKBUF[BUFSZ];
    int cnt; /* do not put on stack, use a register for this */

    /*
     * parse the command line arguments, set mode (encrypt or decrypt)
     *  and open the files
     */
    if (setup(argc, argv, &MODE, &FPBOOK, &FPIN, &FPOUT) == EXIT_FAIL)
        return EXIT_FAILURE;
    
    /*
     * read the input and book file until EOF on the input file
     * Either encrypt or decrypt
     * then write it out.
     */ 
    while ((cnt = rdbuf(IOBUF, BOOKBUF, FPIN, FPBOOK, BUFSZ)) > 0) {
        if (MODE == ENCRYPT_MODE)
            cnt = encrypt(IOBUF, BOOKBUF, cnt);
        else
            cnt = decrypt(IOBUF, BOOKBUF, cnt);
        if ((cnt == EXIT_FAIL) || (wrbuf(IOBUF, cnt, FPOUT) == EXIT_FAIL)) {
            return cleanup(EXIT_FAIL);
        }
    }
    /*
     * close the files
     */
    return cleanup(cnt);
}
