//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     // start of text segment

    //////////////////////////////////////////////////////
    // int decrypt(char *IOBUF, char *BOOKBUF, int cnt) //
    // decrypts  IOBUF using BOOKBUF; result in IOBUF   //
    // 							//
    // Params: IOBUF, BOOKBUF, cnt			//
    // Returns: int					//
    //							//
    // Register Usage:					//
    // scratch registers:				//
    // r0     IOBUF					//
    // r1     BOOKBUF					//
    // r2     cnt					//
    // r3     BOOKBUF byte & decrypted byte copy	//
    //                    				//
    // preserve registers:				//
    // r4     loop counter			        //
    // r5     IOBUF byte & decrypted byte               //
    //////////////////////////////////////////////////////
    .global decrypt
    .type   decrypt, %function
    .equ    FP_OFF, 12

decrypt:
    // function prologue
    push    {r4, r5, fp, lr}          // stack register save
    add     fp, sp, FP_OFF            // set frame pointer to frame base
  
    mov     r4, 0                     // set loop counter to 0
.Lloop:
// for loop to decrypt each byte
    cmp     r4, r2                    // check if loop condition met
    bge     .Ldone                    // if not, exit loop
    ldrb    r5, [r0, r4]              // get IOBUF byte
    ldrb    r3, [r1, r4]              // get BOOKBUF byte
    eor     r5, r5, r3                // exclusive or bytes
    mov     r3, r5                    // copy new byte
    lsl     r5, r5, 28                // isolates bottom half of byte
    lsr     r5, r5, 24                // moves it to top half of first byte
    lsr     r3, r3, 4                 // isolates top half of byte in copy
    orr     r5, r5, r3                // or registers with top and bottom halves
    strb    r5, [r0, r4]              // store decrypted byte back in IOBUF
    add     r4, 1                     // increment loop counter
    b       .Lloop
.Ldone:
// branch to after end of loop
    mov     r0, r2                    // set return value to cnt
    
    // function epilogue
    sub     sp, fp, FP_OFF            // restores stack frame top
    pop     {r4, r5, fp, lr}          // remove frame and restore registers
    bx      lr                        // return to caller

    .size decrypt, (. - decrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
