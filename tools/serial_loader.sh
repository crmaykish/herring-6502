#!/bin/bash

DEVICE=${1}
FILE=${2}

SLEEP_TIME=0.05

echo "Sending ${FILE} to ${DEVICE}"

stty 115200 -F ${DEVICE}

INDEX=0
FILE_SIZE=$(wc -c <${FILE})

DIV=$(($FILE_SIZE / 10))

echo "Program size: "$FILE_SIZE" bytes"

echo -n "Transferring program: "

echo -n "|"

echo -en '\xDE' > ${DEVICE}
sleep $SLEEP_TIME

while [ $INDEX -lt $FILE_SIZE ]; do
    dd if=${FILE} of=${DEVICE} status=none bs=1 count=1 skip=$INDEX

    INDEX=$((INDEX + 1))

    if [ $(($INDEX % $DIV)) == 0 ]; then
        echo -n "-"
    fi

done

echo "|"

echo -en '\xDE' > ${DEVICE}
sleep $SLEEP_TIME
echo -en '\xDE' > ${DEVICE}
sleep $SLEEP_TIME
echo -en '\xDE' > ${DEVICE}
sleep $SLEEP_TIME


echo "Done!"
