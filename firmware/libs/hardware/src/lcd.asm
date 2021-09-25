.export _lcd_init, _lcd_putc, _lcd_clear, _lcd_home, _lcd_print

.include "herring.inc"

.segment "ZEROPAGE"

ptr2: .res 2

.segment "CODE"

_lcd_init:
    pha
    lda #LCD_8_BIT_MODE
    jsr lcd_command
    lda #LCD_DISPLAY_ON
    jsr lcd_command
    lda #LCD_CURSOR_SETTINGS
    jsr lcd_command

    jsr _lcd_clear

    pla
    rts

_lcd_putc:
    pha
    jsr lcd_wait
    pla
    sta LCD_DATA
    rts

_lcd_clear:
    pha
    lda #LCD_CLEAR
    jsr lcd_command
    pla
    rts

_lcd_home:
    pha
    lda #LCD_HOME
    jsr lcd_command
    pla
    rts

_lcd_print:
    pha
    phy
    sta ptr2
    stx ptr2 + 1
    ldy #0
lcd_next_char:
    lda (ptr2),y
    beq lcd_end_string
    jsr _lcd_putc
    iny
    bne lcd_next_char
lcd_end_string:
    ply
    pla

    rts

lcd_wait:
    lda LCD_COMMAND
    and #LCD_BUSY
    bne lcd_wait
    rts

lcd_command:
    pha
    jsr lcd_wait
    pla
    sta LCD_COMMAND
    rts
