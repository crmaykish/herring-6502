; CPU
RESET_VECTOR = $FFFC

; ROM
ROM_START = $C000

; VIA
VIA_PORTB = $8000
VIA_PORTA = $8001
VIA_DDRB = $8002
VIA_DDRA = $8003

; LCD
LCD_DATA_PORT = VIA_PORTA
LCD_CTRL_PORT = VIA_PORTB
LCD_DATA_DDR = VIA_DDRA
LCD_CTRL_DDR = VIA_DDRB

LCD_RS = %0000100
LCD_RW = %0000010
LCD_E =  %0000001

LCD_CMD_INIT = %00110000            ; Set LCD to 8-bit, two-line, 5x8 font
LCD_DISP_SETTINGS = %00001100       ; Display on, cursor on, blink off
LCD_CMD_CLEAR = %00000001           ; Clear the display
LCD_CMD_HOME = %00000010            ; Return cursor to home position