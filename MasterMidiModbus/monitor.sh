#!/bin/sh
PORT=/dev/ttyACM0
if [[ $# -gt 0 ]]
then PORT=$1
fi
echo $PORT
arduino-cli monitor --timestamp -p $PORT -c 9600,8,1
