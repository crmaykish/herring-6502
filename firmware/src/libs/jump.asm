.export _jump_to

.segment "DATA"

target: .res 2

.segment "CODE"

_jump_to:
    sta target
    stx target + 1
    jmp (target)
