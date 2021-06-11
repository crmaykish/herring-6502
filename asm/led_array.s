    .org $8000

    ; LED matrix addresses are 11 bits, data is 6 bits
    ; currently any address in $0000-$7FFF will enable the matrix

    ldx %00111111   ; byte that maps to white on the display controller

    ; I think the address works out such that x & 0b11111100000 is the column and x & 0b11111 is the row

main_loop:
    stx %00001000010    ; write the color in X to [2, 2] in the array
    dex                 ; decrement the color by 1
    jmp main_loop

    .org $fffc
    .word $8000
    .word $0000