.include "herring.inc"

.export _lcd_init, _lcd_instruction, _lcd_putc

.segment "CODE"

_lcd_init:
    ; Set all of the VIA ports to output
    lda #$FF
    sta LCD_DATA_DDR
    sta LCD_CTRL_DDR

    lda #LCD_8_BIT_MODE
    jsr _lcd_instruction

    lda #LCD_DISPLAY_ON
    jsr _lcd_instruction

    lda #LCD_CURSOR_SETTINGS
    jsr _lcd_instruction

    lda #LCD_CLEAR
    jsr _lcd_instruction

    rts

_lcd_instruction:
    jsr lcd_wait
    sta LCD_DATA_PORT
    lda #0
    sta LCD_CTRL_PORT
    lda #LCD_E
    sta LCD_CTRL_PORT
    lda #0
    sta LCD_CTRL_PORT
    rts

_lcd_putc:
    jsr lcd_wait
    sta LCD_DATA_PORT
    lda #LCD_RS
    sta LCD_CTRL_PORT
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL_PORT
    lda #LCD_RS
    sta LCD_CTRL_PORT
    rts

lcd_wait:
    pha
    lda #0
    sta LCD_DATA_DDR
lcdbusy:
    lda #LCD_RW
    sta LCD_CTRL_PORT
    lda #(LCD_RW | LCD_E)
    sta LCD_CTRL_PORT
    lda LCD_DATA_PORT
    and #%10000000
    bne lcdbusy

    lda #LCD_RW
    sta LCD_CTRL_PORT
    lda #$FF
    sta LCD_DATA_DDR
    pla
    rts
