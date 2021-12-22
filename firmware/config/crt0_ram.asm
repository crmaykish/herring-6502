.export _init
.import _main

.export __STARTUP__ : absolute = 1

.import zerobss, initlib

.segment  "STARTUP"

_init:
    ; Clear the BSS section of memory
    jsr zerobss

    ; Run constructors
    jsr initlib

    ; Jump to main()
    jsr _main

    rts
