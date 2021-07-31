.include "herring.inc"
.export _run_loaded_program

.segment "CODE"

_run_loaded_program:
    jsr PROGRAM_RAM
    rts

