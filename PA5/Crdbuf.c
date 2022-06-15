#include <stdio.h>
#include "encrypter.h"

int
rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN, FILE *FPBOOK, int bufsz)
{
    int cnt; /* use in a register no local variables on the stack needed */
   
    if (bufsz <= 0) {
        errmsg("rdbuf: Buffer size error\n");
        return EXIT_FAIL;
    }

    /*
     * read the file
     * cnt should be really a size_t but on ARM32 it is an int
     */
    if ((cnt = (int)fread(IOBUF, 1, bufsz, FPIN)) == 0)
        return cnt;
    
    /*
     * now  read the same number of chars from the bookfile
     * as was read from the input file
     */
    if ((int)fread(BOOKBUF, 1, cnt, FPBOOK) != cnt) {
        errmsg("rdbuf: Bookfile is too short for message\n");
        return EXIT_FAIL;
    }
    /*
     * return the number of chars read
     */
    return cnt;
}
