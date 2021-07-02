ROM_START = $8000
RESET_VECTOR = $FFFC
PORTA = $4001
PORTB = $4000
DDRA = $4003
DDRB = $4002

; LCD
LCD_DATA = PORTB
LCD_CTRL = PORTA

LCD_RS = %0000001
LCD_RW = %0000010
LCD_E =  %0000100

LCD_CMD_INIT = %00110000            ; Set LCD to 8-bit, two-line, 5x8 font
LCD_DISP_SETTINGS = %00001100       ; Display on, cursor on, blink off
LCD_CMD_CLEAR = %00000001           ; Clear the display
LCD_CMD_HOME = %00000010            ; Return cursor to home position

    .org ROM_START

init:
    lda #%11111111                  ; Set all of port B to output
    sta DDRB
    lda #%11111111                  ; Set all pins of port A to output
    sta DDRA

    lda #LCD_CMD_INIT
    jsr lcd_cmd

    lda #LCD_DISP_SETTINGS
    jsr lcd_cmd

loop:
    lda #LCD_CMD_CLEAR
    jsr lcd_cmd

    lda #LCD_CMD_HOME
    jsr lcd_cmd

    ldx #0

display_stuff:
    lda message,x
    beq loop
    jsr lcd_print
    inx
    jmp display_stuff

message: .asciiz "COLIN IS COOL!!!"

lcd_cmd:
    sta LCD_DATA
    lda #0
    sta LCD_CTRL
    lda #LCD_E
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL
    rts

lcd_print:
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #LCD_RS
    sta LCD_CTRL
    rts

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000