ROM_START = $8000
RESET_VECTOR = $FFFC
PORTA = $4001
PORTB = $4000
DDRA = $4003
DDRB = $4002

LCD_DATA = PORTB
LCD_CTRL = PORTA

LCD_RS = %0000001
LCD_RW = %0000010
LCD_E =  %0000100


    .org ROM_START

init:
    lda #%11111111          ; Set all of port B to output
    sta DDRB
    lda #%11111111          ; Set all pins of port A to output
    sta DDRA

    lda #%00110000          ; Set LCD to 8-bit, two-line, 5x8 font
    sta LCD_DATA
    lda #0
    sta LCD_CTRL
    lda #LCD_E
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL

    lda #%00001110          ; Display on, cursor on, blink off
    sta LCD_DATA
    lda #0
    sta LCD_CTRL
    lda #LCD_E
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL

    lda #%00000110          ; Increment and shift cursor
    sta LCD_DATA
    lda #0
    sta LCD_CTRL
    lda #LCD_E
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL

    lda #"C"
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL

    lda #"o"
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL

    lda #"l"
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL

    lda #"i"
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL

    lda #"n"
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL

loop:
    jmp loop

    .org RESET_VECTOR
    .word ROM_START
    .word $0000