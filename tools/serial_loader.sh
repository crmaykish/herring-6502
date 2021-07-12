#!/bin/bash

DEVICE=${1}
FILE=${2}

SLEEP_TIME=0.01

echo "Sending ${FILE} to ${DEVICE}"

stty 9600 -F ${DEVICE}

echo -en '*' >${DEVICE}
sleep $SLEEP_TIME
echo -en '*' >${DEVICE}
sleep $SLEEP_TIME
echo -en '*' >${DEVICE}
sleep $SLEEP_TIME

INDEX=0
FILE_SIZE=$(wc -c <${FILE})

echo "Program size: "$FILE_SIZE" bytes"

while [ $INDEX -lt $FILE_SIZE ]; do
    echo "Write: "$INDEX

    dd if=${FILE} of=${DEVICE} status=none bs=1 count=1 skip=$INDEX 

    INDEX=$((INDEX + 1))

    sleep $SLEEP_TIME
done

echo -en '!' >${DEVICE}
sleep $SLEEP_TIME
echo -en '!' >${DEVICE}
sleep $SLEEP_TIME
echo -en '!' >${DEVICE}
sleep $SLEEP_TIME

echo "Done"
