.extern START      /* Error: The label START is defined within the file, so it should not be declared as external. */
.entry END         /* Error: The label END is declared as an entry point but is not used as a label. */
.entry LOOP        /* Error: The label LOOP is declared as an entry point but is not defined anywhere in the file. */

START:  mov r1, r2
        add r3, r4
        jsr PROCESS

LOOP:   cmp r2, r3
        bne NEXT
        prn r1
        jmp END

NEXT:   dec r1
        jmp LOOP

PROCESS: sub r4, r1
         rts

END:    stop
.extern DATA1      
.extern FUNC1      

MAIN:   mov r1, r2
        add r3, r4
        jsr PROCESS

PROCESS: sub r4, r1
         rts

DATA2:  .data 5, 10, -15
STR:    .string "hello"

