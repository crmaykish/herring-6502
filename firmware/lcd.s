; LCD
LCD_DATA = PORTB
LCD_CTRL = PORTA

LCD_RS = %0000001
LCD_RW = %0000010
LCD_E =  %0000100

LCD_FUNC_SET = %00110110            ; 8-bit interface, extended instruction set, graphic display on
LCD_DISP_SETTINGS = %00001100       ; Display on, cursor on, blink off

; Clear screen command
LCD_CLEAR = %00000001           
LCD_SET_GD_ADDR = %10000000

lcd_init:
    lda #%11111111                  ; Set all of port B to output
    sta DDRB
    lda #%11111111                  ; Set all pins of port A to output
    sta DDRA

    lda #0                          ; Reset control flags
    sta LCD_CTRL

    jsr lcd_function_set
    jsr lcd_function_set

    jsr lcd_clear

    ; Entry mode set
    lda #%00000110
    jsr lcd_send_command

    ; Home
    lda #%00000010
    jsr lcd_send_command

    rts

; Clear the screen
lcd_clear:
    ; Command value to clear screen
    lda #%00000001
    jsr lcd_send_command
    rts

; Send a single command to the LCD
; A is the command to send
lcd_send_command:
    sta LCD_DATA
    lda #0
    sta LCD_CTRL
    lda #LCD_E
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL
    rts

; Set the internal RAM address to A
; vertical coordinate (0 - 31)
; horizontal coordinate (0 - 15)
lcd_set_ram_address:
    ora #%10000000
    jsr lcd_send_command
    rts

; Write a byte to the LCD internal RAM
; Note: set the internal RAM address first
lcd_write:
    sta LCD_DATA
    lda #LCD_RS
    sta LCD_CTRL
    lda #(LCD_RS | LCD_E)
    sta LCD_CTRL
    lda #0
    sta LCD_CTRL
    rts

lcd_function_set:
    lda #%00110000
    jsr lcd_send_command
    rts

lcd_function_set_extended:
    lda #%00110100
    jsr lcd_send_command
    rts

lcd_function_set_extended_graphics:
    lda #%00110110
    jsr lcd_send_command
    rts