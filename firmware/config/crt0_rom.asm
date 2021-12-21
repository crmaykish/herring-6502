.export _init
.import _main

.export __STARTUP__ : absolute = 1
.import __RAM_START__, __RAM_SIZE__

.import copydata, zerobss, initlib

.include "zeropage.inc"

.segment "VECTORS"
     .addr nmi_handler
     .addr _init
     .addr irq_handler

.segment  "STARTUP"

_init:
    jmp start

nmi_handler:
    jmp error_catch

irq_handler:
    jmp error_catch

; In the event of errors, stop everything and loop forever
error_catch:
    jmp error_catch

start:
    ; Disable interrupts
    sei

    ; Start the stack pointer at 0x01FF
    ldx #$FF
    txs

    ; Clear decimal mode
    cld

    lda #<(__RAM_START__ + __RAM_SIZE__)
    sta sp
    lda #>(__RAM_START__ + __RAM_SIZE__)
    sta sp+1

    ; Clear the BSS section of memory
    jsr zerobss
    
    ; Copy mutable data from ROM to RAM
    jsr copydata
    
    ; Run constructors
    jsr initlib

    ; Jump to main()
    jsr _main

    ; If main() ever exits, something terrible happened
    jmp error_catch
