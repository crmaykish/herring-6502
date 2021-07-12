    .export _run_loaded_code

    PROGRAM_START = $1000

_run_loaded_code:
    jmp PROGRAM_START