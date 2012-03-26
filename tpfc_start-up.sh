#!/bin/bash
sleep 15 &&
TP_PATH=`pwd`
TP_PATH='/home/stanko/Projects/fans/'
cd $TP_PATH
gksu data/tp_fan_control &
exit
