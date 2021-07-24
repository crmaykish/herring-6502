.export   _init
.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import   __RAM_START__, __RAM_SIZE__       ; Linker generated

.import    copydata, zerobss, initlib

.include  "zeropage.inc"

.segment "VECTORS"

    .addr nmi_handler
    .addr _init
    .addr irq_handler

.segment  "STARTUP"

_init:
    ; Set up the VIA immediately so we can display error codes
    lda #$FF
    sta $C002
    sta $C003

    jmp start

; NMIs should never happen, stop everything
nmi_handler:
    lda #%11110000
    sta $C001
    jmp error_catch

; Normal interrupts should be disabled, if we see one, set the LED pattern and return
irq_handler:
    lda #%00000111
    sta $C001
    rti

error_catch:
    jmp error_catch

start:
    SEI    
    LDX     #$FF                 ; Initialize stack pointer to $01FF
    TXS
    CLD                          ; Clear decimal mode

    lda #1
    sta $C001

    LDA     #<(__RAM_START__ + __RAM_SIZE__)
    STA     sp
    LDA     #>(__RAM_START__ + __RAM_SIZE__)
    STA     sp+1

    lda #2
    sta $C001

    ; JSR     zerobss              ; Clear BSS segment

    lda #3
    sta $C001

    JSR     copydata             ; Initialize DATA segment

    lda #4
    sta $C001

    JSR     initlib              ; Run constructors

    lda #5
    sta $C001

    JSR     _main

    lda #6
    sta $C001

    jmp error_catch