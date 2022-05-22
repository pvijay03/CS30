    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    // include files
    #include "cw.h"

    // name imports
    .extern     cntchar
    .extern     cntword
    .extern     cntline
    .extern     printf

    .section .rodata
.Lmsg:
    .string "lines:%8.8d words:%8.8d Chars:%8.8d\n"

    .text
    //////////////////////////////////////////////////////
    // void result(void)                                //
    //////////////////////////////////////////////////////

    .global result              // global for linking to
    .type   result, %function   // define as a function
    .equ    FP_OFF, 4           // fp offset in main stack frame

result:
    // function prologue
    push    {fp, lr}            // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base
    // do not edit above this line

    // print out the counts from the global variables
    /*
     * (0) put the address of .Lmsg in r0
     * (1) put the address of cntline in r1
     * (2) put the address of cntword in r2
     * (3) put the address of cntchar in r3
     */
    ldr     r0, =.Lmsg          // puts address of .Lmsg in r0
    ldr     r1, =cntline        // puts address of cntline in r1
    ldr     r2, =cntword        // puts address of cntword in r2
    ldr     r3, =cntchar        // puts address of cntchar in r3
    ldr     r1, [r1]            // puts what cntline is pointing to in r1
    ldr     r2, [r2]            // puts what cntword is pointing to in r2
    ldr     r3, [r3]            // puts what cntchar is pointing to in r3
    bl      printf              // calls printf function
    
    // do not edit below this line
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {fp, lr}            // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   result, (. - result)// set size for function
.end
