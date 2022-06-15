//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.01 05/27/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    // imports
    .extern stderr
    .extern fprintf

    .text

    ///////////////////////////////////////////////
    // void errmsg(char *errormsg)               //
    // writes error messages to stder            //
    //                                           //
    // Params: errormsg                          //
    // Returns: void                             //
    //                                           //
    // Register Usage:                           //
    // r0     fprintf first param, stderr file   //
    // r1     fprintf second param, char pointer //
    ///////////////////////////////////////////////
    .type   errmsg, %function
    .global errmsg
    .equ    FP_OFF,  4

errmsg:
    // function prologue
    push    {fp, lr}                    // stack register save
    add     fp, sp, FP_OFF              // set frame pointer to frame base

    mov     r1, r0                      // sets second param to passed argument
    ldr     r0, =stderr                 // loads address of stderr
    ldr     r0, [r0]                    // sets first param to stderr file
    bl      fprintf

    // function epilogue
    sub     sp, fp, FP_OFF              // restore stack frame top
    pop     {fp, lr}                    // remove frame and restore registers
    bx      lr                          // return to caller

    .size errmsg, (. - errmsg)
    .section .note.GNU-stack,"",%progbits
.end
#endif
