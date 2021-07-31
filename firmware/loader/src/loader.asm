.include "herring.inc"
.export _run_loaded_program

.segment "CODE"

_run_loaded_program:
    jmp PROGRAM_RAM
    rts

