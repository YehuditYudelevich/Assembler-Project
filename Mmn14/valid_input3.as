.extern externalFunc1
.extern externalVar1
.entry start

start:       mov r2, r3
             add r1, externalVar1
             prn #25
             jsr externalFunc1
             clr r4
             sub r5, r6
             lea string1, r7
             jmp loop

label1:      .data 100, 200, -300, 400
string1:     .string "Hello, World!"

loop:        cmp r3, #-15
             bne next
             inc r4
             dec r5
             prn label1
             jsr externalFunc1
             stop

next:        lea label1, r6
             mov r7, externalVar1
             add r1, r2
             sub r3, r4
             clr r7
             prn r6
             jmp start

.entry label1
.extern externalFunc2
.extern externalVar2
 stop