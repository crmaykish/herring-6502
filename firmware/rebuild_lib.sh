#!/bin/sh

LIB_TYPE=${1}

SOURCE_LIB="/usr/share/cc65/lib/supervision.lib"
DEST_LIB="config/${LIB_TYPE}.lib"
CRT_ASM="config/crt0_${LIB_TYPE}.asm"
CRT_O="config/crt0.o"

echo "Removing existing lib file"
rm -f config/${LIB_TYPE}.lib

echo "Copy supervision.lib"
cp ${SOURCE_LIB} ${DEST_LIB}

cp $CRT_ASM config/crt0.asm

echo "Assemble crt0.asm"
ca65 --cpu 65c02 config/crt0.asm --include-dir libs/hardware/include/

echo "Add crt0.o to lib file"
ar65 a ${DEST_LIB} ${CRT_O}

rm config/crt0.asm
rm ${CRT_O}
