//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/22/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "encrypter.h"

    .section .rodata
.Lmsg:  .string "rdbuf: Bookfile is too short for message\n"
.Lmsg2: .string "rdbuf: Buffer size error\n"

    .text

    ///////////////////////////////////////////////////////
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN  //
    //              FILE *FPBOOK, int BUFSZ);            //
    // returns number of bytes read, EXIT_FAIL otherwise //
    //							 //
    // Params: IOBUF, BOOKBUF, FPIN, FPBOOK, BUFSZ       //
    // Returns: int					 // 
    ///////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    .equ    IARG5,  4 
    .equ    FP_OFF, 12
 
    //////////////////////////////////////////////
    // scratch register use                     //
    // r0 address of message, IOBUF, BOOKBUF    //
    // r1 1 					//
    // r2 bufsz & cnt				//
    // r3 FPIN & FPBOOK                         //
    //						//
    // preserved register use                   //
    // r4 pointer to book buffer & bufsz        //
    // r5 FILE *FPBOOK & cnt read saved         //
    //////////////////////////////////////////////

rdbuf:
    // function prologue
    push    {r4, r5, fp, lr}          // stack register save
    add     fp, sp, FP_OFF            // set frame pointer to frame base

    ldr     r4, [fp, IARG5]           // gets bufsz from stack
    cmp     r4, 0                     // checks if bufsz is valid
    bgt     .Lsecondif                // if it is, no error, branch around
    ldr     r0, =.Lmsg2               // sets first param as buffer error msg
    bl      errmsg
    mov     r0, EXIT_FAIL             // sets return value to EXIT_FAIL
    b       .Ldone
.Lsecondif:
// branch to if bufsz valid
    mov     r4, r1                    // preserves BOOKBUF
    mov     r5, r3                    // preserves FPBOOK
    mov     r1, 1                     // sets second param to 1
    mov     r3, r2                    // sets fourth param to FPIN
    ldr     r2, [fp, IARG5]           // sets third param to bufsz
    bl      fread
    cmp     r0, 0                     // checks if bytes were read
    beq     .Ldone                    // if not, function ends, branch to end
    mov     r2, r0                    // sets third param to cnt
    mov     r3, r5                    // sets fourth param to FPBOOK
    mov     r5, r0                    // preserves cnt
    mov     r0, r4                    // sets first param to BOOKBUF
    mov     r1, 1                     // sets second param to 1
    bl      fread
    cmp     r0, r5                    // checks if bytes read equals cnt
    beq     .Lthirdif                 // if they are, branch around error
    ldr     r0, =.Lmsg                // sets first param as bookfile error msg
    bl      errmsg
    mov     r0, EXIT_FAIL             // sets return value to EXIT_FAIL
    b       .Ldone
.Lthirdif:
// branch to if bytes read is correct
    mov     r0, r5                    // sets return value to cnt
.Ldone:
    // function epilogue
    sub     sp, fp, FP_OFF            // restore stack frame top
    pop     {r4, r5, fp, lr}          // remove frame and restore registers
    bx      lr                        // return to caller

    .size   rdbuf, (. - rdbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
