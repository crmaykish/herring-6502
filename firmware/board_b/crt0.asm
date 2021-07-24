.export   _init
.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import   __RAM_START__, __RAM_SIZE__       ; Linker generated

.import    copydata, zerobss, initlib

.include  "zeropage.inc"

.segment "VECTORS"

    .addr bad_stuff
    .addr _init
    .addr bad_stuff

.segment  "STARTUP"

_init:
    jmp start

bad_stuff:
    lda #$55
    sta $C000
stuck:
    jmp stuck

start:
    SEI    
    LDX     #$FF                 ; Initialize stack pointer to $01FF
    TXS
    CLD                          ; Clear decimal mode

    ; set up the via so we can see error codes
    lda #$FF
    sta $C002
    sta $C003

    lda #1
    sta $C000

    LDA     #<(__RAM_START__ + __RAM_SIZE__)
    STA     sp
    LDA     #>(__RAM_START__ + __RAM_SIZE__)
    STA     sp+1

    lda #2
    sta $C000

    ; JSR     zerobss              ; Clear BSS segment

    lda #3
    sta $C000

    JSR     copydata             ; Initialize DATA segment

    lda #4
    sta $C000

    JSR     initlib              ; Run constructors

    lda #5
    sta $C000

    JSR     _main

    lda #6
    sta $C000

    jmp bad_stuff