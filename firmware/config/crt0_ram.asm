.export _init
.import _main

.export __STARTUP__ : absolute = 1
.import __RAM_START__, __RAM_SIZE__

.import zerobss, initlib

.include "zeropage.inc"
.include "herring.inc"

.segment  "STARTUP"

_init:
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
    
    ; Run constructors
    jsr initlib

    ; Jump to main()
    jsr _main

    ; If main() exits, attempt to return from subroutine
    ; TODO: probably won't work, this program will blow away the old stack
    rts
