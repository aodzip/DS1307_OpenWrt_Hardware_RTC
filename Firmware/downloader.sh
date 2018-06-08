#!/bin/bash
while [ "1"="1" ]
    do
        echo 'Wait ACK...';
        read;
        echo 'activate this console.' > /dev/ttyUSB0
        echo 'Wait Time Request';
        read;
        echo `date -u '+%y.%m.%d-%H:%M:%S'` > /dev/ttyUSB0
        echo > /dev/ttyUSB0
    done
    