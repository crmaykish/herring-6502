.export _init
.import _main

.export __STARTUP__ : absolute = 1
.import __RAM_START__, __RAM_SIZE__

.import copydata, zerobss, initlib

.include "zeropage.inc"
.include "herring.inc"

; Error codes
NMI_CODE = %11110000
IRQ_CODE = %00001111
END_OF_MAIN_CODE = %00111100
ERROR_PORT = VIA0_PORTA

.segment "VECTORS"
     .addr nmi_handler
     .addr _init
     .addr irq_handler

.segment  "STARTUP"

_init:
    ; Set up the VIA immediately so we can display error codes
    lda #VIA_OUTPUT
    sta VIA0_DDRA
    sta VIA0_DDRB
    lda #0
    sta VIA0_PORTB
    sta ERROR_PORT
    jmp start

nmi_handler:
    lda #NMI_CODE
    sta ERROR_PORT
    jmp error_catch

irq_handler:
    lda #IRQ_CODE
    sta ERROR_PORT
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
    
    lda #1
    sta ERROR_PORT

    ; Copy mutable data from ROM to RAM
    jsr copydata
    
    lda #2
    sta ERROR_PORT

    ; Run constructors
    jsr initlib

    lda #3
    sta ERROR_PORT

    ; Jump to main()
    jsr _main

    ; If main() ever exits, something terrible happened
    lda #END_OF_MAIN_CODE
    sta ERROR_PORT
    jmp error_catch
