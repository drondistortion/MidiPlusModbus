#!/bin/sh
PORT=/dev/ttyACM0
if [[ $# -gt 0 ]]
then PORT=$1
fi
arduino-cli upload -b arduino:avr:leonardo -v -p $PORT
date
