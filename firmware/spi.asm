    .export _SPI_Init, _SPI_ReadByte, _SPI_WriteByte

    MISO = %10000000
    MOSI = %01000000
    SCK =  %00100000
    CS =   %00010000

    SPI_OUTPUT_PINS = MOSI | SCK | CS

    SPI_DDR = $C400
    SPI_PORT = $C402

    .segment "CODE"

_SPI_Init:
    lda #SPI_OUTPUT_PINS
    sta SPI_DDR
    lda #CS | MOSI
    ldx #160               ; toggle the clock 160 times, so 80 low-high transitions
preinitloop:
    eor #SCK
    sta SPI_PORT
    dex
    bne preinitloop

    rts

_SPI_ReadByte:
    ldx #8                      ; we'll read 8 bits
loop:

    lda #MOSI                ; enable card (CS low), set MOSI (resting state), SCK low
    sta SPI_PORT

    lda #MOSI | SCK       ; toggle the clock high
    sta SPI_PORT

    lda SPI_PORT                   ; read next bit
    and #MISO

    clc                         ; default to clearing the bottom bit
    beq bitnotset              ; unless MISO was set
    sec                         ; in which case get ready to set the bottom bit
bitnotset:

    tya                         ; transfer partial result from Y
    rol                         ; rotate carry bit into read result
    tay                         ; save partial result back to Y

    dex                         ; decrement counter
    bne loop                   ; loop if we need to read more bits

    rts

_SPI_WriteByte:
    rts