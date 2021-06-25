    .include herring_zero.s
    .org ROM_START

init:
    lda #%11111111                  ; Set all of port A to output
    sta LCD_DATA_DDR
    lda #%00000111                  ; Set low three pins of port B to output
    sta LCD_CTRL_DDR

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
    sta LCD_DATA_PORT
    lda #0
    sta LCD_CTRL_PORT
    lda #LCD_E
    sta LCD_CTRL_PORT
    lda #0
    sta LCD_CTRL_PORT
    rts

lcd_print:
    sta LCD_DATA_PORT
    lda #LCD_RS
    sta LCD_CTRL_PORT
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL_PORT
    lda #LCD_RS
    sta LCD_CTRL_PORT
    rts

message: .asciiz "HERRING ZERO"

    ; .org RESET_VECTOR
    ; .word ROM_START
    ; .word $0000
