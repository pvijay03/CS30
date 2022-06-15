//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified
    
    // import constants
    #include "encrypter.h"

    .section .rodata
.Lmsg: .string "wrbuf: Write failed on output\n"
.Lmsg2: .string "wrbuf: Buffer size error\n"

    .text

    ///////////////////////////////////////////////////
    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT); //
    // return cnt on success EXIT_FAIL otherwise     //
    //						     //
    // Params: IOBUF, cnt, FPOUT		     //
    // Returns: int			  	     //
    //        					     //
    // Register Usage:                               //
    // scratch registers:                            //
    // r0     address of message, IOBUF  	     //
    // r1     1					     //
    // r2     cnt	                             //
    // r3     FPOUT			             //
    //                                               //
    // preserve registers:                           //
    // r4     cnt                                    //
    // r5     not needed                             //
    ///////////////////////////////////////////////////
    .type   wrbuf, %function
    .global wrbuf
    .equ    FP_OFF, 12
 
wrbuf:
    // function prologue
    push    {r4, r5, fp, lr}          // stack register save
    add     fp, sp, FP_OFF            // set frame pointer to frame base

    cmp     r1, 0   		      // check if cnt is valid
    bgt     .Lnextif                  // if it is, no error, branch around
    ldr     r0, =.Lmsg2               // sets first param as bufsz error msg
    bl      errmsg
    mov     r0, EXIT_FAIL	      // sets return value to EXIT_FAIL
    b       .Ldone
.Lnextif:
// branch to if cnt is valid
    mov     r3, r2		      // sets fourth param to FPOUT
    mov     r2, r1		      // sets third param to cnt
    mov     r1, 1		      // sets second param to 1
    mov     r4, r2                    // preserves cnt
    bl      fwrite
    cmp     r0, r4                    // checks of bytes written equals cnt
    beq     .Lfinal                   // if they are, branch around error
    ldr     r0, =.Lmsg                // sets first param as write error msg
    bl      errmsg
    mov     r0, EXIT_FAIL             // sets return value to EXIT_FAIL
    b       .Ldone
.Lfinal:
// branch to if bytes written is correct
    mov     r0, r4                    // sets return value to cnt
.Ldone:
    // function epilogue
    sub     sp, fp, FP_OFF            // restore stack frame top
    pop     {r4, r5, fp, lr}          // remove frame and restore registers
    bx      lr                        // return to caller

    .size   wrbuf, (. - wrbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
