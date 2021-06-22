    .include herring.s
    .org ROM_START

    .include lcd.s

init:
    jsr lcd_init
    jsr lcd_function_set_extended
    jsr lcd_function_set_extended_graphics

restart:
    jsr lcd_clear
    ; TODO: clear graphics ram
    ldx #0
    ldy #0

loop:
    txa
    jsr lcd_set_ram_address
    tya
    jsr lcd_set_ram_address

    lda #%00001111
    jsr lcd_write

    inx
    iny

    clc

    cpx #32
    bcs restart

    cpy #16
    bcs restart

    jmp loop
