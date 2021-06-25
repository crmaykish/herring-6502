    .include herring_zero.s
    .org ROM_START

init:
    lda #%11111111                  ; Set all of port A to output
    sta VIA_DDRA
    lda #%00000111                  ; Set low three pins of port B to output
    sta VIA_DDRB

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

_loop_display
    lda message,x
    beq loop
    jsr lcd_print
    inx
    jmp _loop_display

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

message: .asciiz "HERRING ZERO"

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000
