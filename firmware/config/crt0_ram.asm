.export _init
.import _main

.export __STARTUP__ : absolute = 1

.import initlib

.segment  "STARTUP"

_init:
    ; Run constructors
    jsr initlib

    ; Jump to main()
    jsr _main

    rts
