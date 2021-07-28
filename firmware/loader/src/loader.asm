.include "herring.inc"

.export _load_from_serial, _run_loaded_program

.segment "ZEROPAGE"

ptr1: .res 2

.segment "CODE"

_load_from_serial:
    rts

_run_loaded_program:
    jsr PROGRAM_RAM
    rts
