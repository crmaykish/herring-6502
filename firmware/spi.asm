    ; Adapted from (i.e. blatantly stolen from) https://github.com/gfoot/sdcard6502

    .export _SPI_Init, _SPI_ReadByte, _SPI_WriteByte, _SPI_WaitResult

    SPI_PORT = $C401
    SPI_DDR = $C403

    CLK = %00000001
    MISO = %00000010
    MOSI = %00000100
    CS = %00001000

    .segment "CODE"

_SPI_Init:
    ; Set the outputs
    lda #(CS | MOSI | CLK)
    sta SPI_DDR
    lda #(CS | MOSI)
    ldx #160
toggle_loop:
    eor #CLK
    sta SPI_PORT
    dex
    bne toggle_loop
    rts

_SPI_ReadByte:
    ldx #8
readloop:
    lda #MOSI
    sta SPI_PORT

    lda #(MOSI | CLK)
    sta SPI_PORT

    lda SPI_PORT
    and #MISO

    clc
    beq bitnotset
    sec
bitnotset:
    tya
    rol
    tay
    dex
    bne readloop

    rts

_SPI_WriteByte:
    ldx #8

writeloop:
    asl
    tay
    lda #0
    bcc sendbit
    ora #MOSI
sendbit:
    sta SPI_PORT
    eor #CLK
    sta SPI_PORT
    tya
    dex
    bne writeloop

    rts

_SPI_WaitResult:
    jsr _SPI_ReadByte
    cmp #$FF
    beq _SPI_WaitResult
    rts