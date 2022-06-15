//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // function import
    .extern encrypt
    .extern decrypt
    .extern setup
    .extern rdbuf
    .extern cleanup
    .extern wrbuf

    // global constants
    #include "encrypter.h"

    .text

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // encrypter [-d | -e] -b bookfile encryption_file  //
    //							//
    // Params: argc, argv				//
    // Returns: int					//
    //////////////////////////////////////////////////////

    // finish the stack frame first
    .global main
    .type   main, %function
    .equ    BUFSZ,      1024           // buffer size
    .equ    FP_OFF,     28             // fp offset in main stack frame
    .equ    FPBOOK,     4 + FP_OFF     // FILE * to book file
    .equ    FPIN,       4 + FPBOOK     // FILE * to input file
    .equ    FPOUT,      4 + FPIN       // FILE * to output file
    .equ    MODE,       4 + FPOUT      // decrypt or encrypt mode
    .equ    IOBUF,      BUFSZ + MODE   // buffer for input file
    .equ    BOOKBUF,    BUFSZ + IOBUF  // buffer for book file
    // add local variables here: adjust PAD size
    .equ    PAD,        0 + BOOKBUF    // Stack frame PAD 
    .equ    OARG6,      4 + PAD        // output arg6
    .equ    OARG5,      4 + OARG6      // output arg5 must be at bottom
    .equ    FRAMESZ,    OARG5 - FP_OFF // total space for frame
 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to input/output buffer  //
    // r5 pointer to book buffer          //
    // r6 mode flag                       //
    // r7 input FILE *                    //
    // r8 book FILE *                     //
    // r9 output FILE *                   //
    ////////////////////////////////////////

main:
    // function prologue
    push    {r4-r9, fp, lr}         // stack register save
    add     fp, sp, FP_OFF          // set frame pointer to frame base
    ldr     r3, =FRAMESZ            // frame size is big, use pseudo ldr
    sub     sp, sp, r3              // allocate space for locals and passed args 
     
    add     r6, fp, -MODE           // preserves address of MODE
    mov     r2, r6                  // sets third param to address of MODE
    add     r8, fp, -FPBOOK         // preserves address of FPBOOK
    mov     r3, r8                  // sets fourth param to address of FPBOOK
    add     r7, fp, -FPIN           // preserves address of FPIN
    str     r7, [fp, -OARG5]        // sets fifth param to address of FPIN
    add     r9, fp, -FPOUT          // preserves address of FPOUT
    str     r9, [fp, -OARG6]        // sets sixth param to address of FPOUT
    bl      setup
    cmp     r0, EXIT_FAIL           // checks if setup failed
    beq     .Ldone
    ldr     r6, [r6]                // preserves MODE
    
.Lloop:
// loop until EOF on FPIN
    ldr     r0, =BUFSZ              // gets BUFSZ with pseudo ldr
    str     r0, [fp, -OARG5]        // sets fifth param as BUFSZ
    ldr     r4, =IOBUF              // gets IOBUF with pseudo ldr
    sub     r4, fp, r4              // gets pointer to IOBUF
    mov     r0, r4                  // sets first param to address of IOBUF
    ldr     r5,  =BOOKBUF           // gets BOOKBUF with psuedo ldr
    sub     r5, fp, r5              // gets pointer to BOOKBUF
    mov     r1, r5                  // sets second param to address of BOOKBUF
    ldr     r2, [r7]                // sets third param to FPIN
    ldr     r3, [r8]                // sets fourth param to FPBOOK 
    bl      rdbuf
    cmp     r0, 0                   // checks if loop condition met
    ble     .Ldone                  // if not, function ends
.Lprocinput:
// decrypts or encrypts input
    mov     r2, r0                  // sets third param to cnt
    mov     r0, r4                  // sets first param to IOBUF
    mov     r1, r5                  // sets second param to BOOKBUF
    cmp     r6, ENCRYPT_MODE        // checks the mode
    bne     .Ldecrypt               // if it is decrypt, branch to decrypt
    bl      encrypt
    b       .Lwriteout
.Ldecrypt: 
// decrypt the input
    bl      decrypt
.Lwriteout:
// writes out buffer
    cmp     r0, EXIT_FAIL           // checks if cnt is valid
    beq     .Ldone                  // if not, function ends
    mov     r1, r0                  // sets second param to cnt
    mov     r0, r4                  // sets first param to IOBUF
    ldr     r2, [r9]                // sets third param to FPOUT
    bl      wrbuf
    cmp     r0, EXIT_FAIL           // checks if wrbuf failed
    beq     .Ldone                  // if it did, function ends
    b       .Lloop
.Ldone:
// branch to after end of loop
    bl      cleanup
.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF          // restores stack frame top
    pop     {r4-r9, fp, lr}         // remove frame and restore register
    bx      lr                      // return to caller

    .size   main, (. - main)
    .section .note.GNU-stack,"",%progbits
.end
#endif
