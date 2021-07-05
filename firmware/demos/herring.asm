ROM_START = $8000
RESET_VECTOR = $FFFC

; VIA
PORTA = $4001
PORTB = $4000
DDRA = $4003
DDRB = $4002

; ACIA
ACIA_DATA = $6000
ACIA_STATUS = $6001
ACIA_COMMAND = $6002
ACIA_CONTROL = $6003

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
