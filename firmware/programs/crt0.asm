.export   _init, _exit
.import   _main

.export   __STARTUP__ : absolute = 1        ; Mark as startup
.import   __RAM_START__, __RAM_SIZE__       ; Linker generated

.import    zerobss, initlib, donelib

.include  "zeropage.inc"

.segment  "STARTUP"

_init:   
          LDX     #$FF                 ; Initialize stack pointer to $01FF
          TXS

          LDA     #<(__RAM_START__ + __RAM_SIZE__)
          STA     sp
          LDA     #>(__RAM_START__ + __RAM_SIZE__)
          STA     sp+1

          JSR     zerobss              ; Clear BSS segment
          JSR     initlib              ; Run constructors

          JSR     _main

_exit:    JSR     donelib              ; Run destructors
          BRK