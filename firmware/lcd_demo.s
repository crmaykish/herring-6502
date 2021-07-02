    .include herring.s

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

message: .asciiz "Herring 6502"

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