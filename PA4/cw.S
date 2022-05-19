    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    //  include files
    #include    "cw.h"

    // name imports
    .extern     setup
    .extern     rd
    .extern     result
    
    // name exports
    .global     buffer
    .global     cntchar
    .global     cntword
    .global     cntline

    .data       // segment
    // allocate space for (1) buffer, (2) cntchar (3) cntword
    // (4) cntline; make sure to watch alignments!
cntchar:     .word 0   // count of valid chars
cntword:     .word 0   // count of words
cntline:     .word 0   // count of new line chars
buffer:      .space BUFSZ   // array of chars

    .text       // segment

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // program usage: cw [file]                         //
    //                                                  //
    // you can use any scratch register r0-r3           //
    // you can use these preserved registers            //
    // r0     contains return value of rd()             //
    // r1     pointer to beginning of buffer            //
    // r2     pointer to end of buffer                  //
    // r3     during loop: store chars                  //
    //        after loop: fills the following r4-r6     //
    // r4     cntchar                                   //
    // r5     cntword                                   //
    // r6     cntline                                   //
    // r7     new word                                  //
    //////////////////////////////////////////////////////

    .global main                // global for linking to
    .type   main, %function     // define as a function
    .equ    EXIT_SUCCESS, 0
    .equ    EXIT_FAILURE, 1
    .equ    FP_OFF, 20          // fp offset in stack frame

main:
    // function prologue
    // do not edit
    push    {r4-r7,fp, lr}      // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    bl      setup               // call setup(argc, argv)
    cmp     r0, 0               // returns 0 if ok, -1 otherwise
    beq     .Lok
    mov     r0, EXIT_FAILURE
    b       .Lexit

.Lok:
    // can edit from here


/*
 * in a loop call rd() until end of file
 * add your code to
 * (1)  count the number of characters in the words
 * (not including any spaces, newlines or tabs,
 * (2) the number of words. words are delimited by space, \n or \t
 * (3) the number of \n in the input
 */

// here is how to use rd()
    //bl      rd                // read another buffer
    //cmp     r0, 0             // if r0 <= 0 then EOF
    //ble     .Ldone

// your code here
    ldr    r4, =0               // sets cntchar register to 0
    ldr    r5, =0               // sets cntword register to 0
    ldr    r6, =0               // sets cntline register to 0
.Louterloop:
// in a loop until end of file
    bl     rd                   // fills buffer and sets r0
    cmp    r0, 0                // checks if buffer at EOF
    ble    .Ledge               // if it is, branch out of loop
    ldr    r1, =buffer          // sets pointer to start of buffer
    add    r2, r1, r0           // sets pointer to end of buffer
    cmp    r1, r2               // checks if pointer is at end pointer
    beq    .Louterloop          // if it is, branch to outer loop
.Linnerloop:
// in a loop until end of buffer
    cmp    r1, r2               // checks if pointer is at end pointer 
    beq    .Louterloop          // if it is, branch to outer loop
    ldrb   r3, [r1]             // puts current char pointed to in r3
    cmp    r3, ' '              // checks if char is ' '
    beq    .Lfirstif            // if it is, branch to first if true block
    cmp    r3, '\t'             // checks if char is '\t'
    beq    .Lfirstif            // if it is, branch to first if true block
    cmp    r3, '\n'             // checks if char is '\n'
    bne    .Lelse               // if it is not, branch to else false block
    b      .Lsecondif           // branch to second if true block
.Lfirstif:
// first if true block
    add    r1, r1, 1            // increment pointer
    cmp    r7, 0                // checks if new word is 0
    bne    .Lfirstinner         // if it is not branch to true block
    b      .Linnerloop          // branch to inner loop
.Lfirstinner:
// first inner true block
    add    r5, r5, 1            // increment cntword register
    mov    r7,  0               // sets new word to 0
    b      .Linnerloop          // branch to inner loop
.Lsecondif:
// second if true block
    add    r1, r1, 1            // increment pointer
    add    r6, r6, 1            // increment cntline register
    cmp    r7, 1                // checks if new word is 1
    bne    .Lsecondinner        // if it is not, branch to inner true block
    add    r5, r5, 1            // increment cntword register
.Lsecondinner:
// second inner true block
    mov    r7, 0                // sets new word to 0
    b        .Linnerloop        // branch to inner loop
.Lelse:
// else false block
    add    r1, r1, 1            // increment pointer
    add    r4, r4, 1            // increment cntchar register
    mov    r7, 1                // sets new word to 1
    b      .Linnerloop          // branch to inner loop
.Ledge:
// out of loop edge case
    cmp    r7, 1                // checks if new word is 1
    bne    .Ldone               // if it is not, branch to done
    add    r5, 1                // increment cntword register
.Ldone:
// checking and counting is done, set variables
    ldr    r3, =cntchar        // gets address of cntchar
    str    r4, [r3]            // sets cntchar to counted chars
    ldr    r3, =cntword        // gets address of cntword
    str    r5, [r3]            // sets cntword to counted words
    ldr    r3, =cntline        // gets address of cntline
    str    r6, [r3]            // sets cntline to counted new lines
    // do not edit from here to end of this file
    bl      result              // print the results
    mov     r0, EXIT_SUCCESS    // fall through to .Lexit
.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r7,fp,lr}       // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function
.end
