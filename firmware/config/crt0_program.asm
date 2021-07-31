.export   _init
.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import   __RAM_START__, __RAM_SIZE__       ; Linker generated

.import    zerobss, initlib, donelib

.include  "zeropage.inc"

.segment  "STARTUP"

_init:
    jsr initlib
    jsr _main
    jsr donelib
    rts
