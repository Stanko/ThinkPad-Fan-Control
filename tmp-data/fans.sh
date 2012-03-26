#!/bin/bash
#podesi vremena i temperature kako ti odgovaraju

#mrtva petlja u kojoj se sve desava
while true; do

#cupam trenutnu temperaturu procesora
TEMP=`cat /proc/acpi/ibm/thermal | awk '{print $2}'`
	
	if [ "$TEMP" -ge "60" ]; then
		#ako je veca od 60 startujemo full
		echo level full-speed > /proc/acpi/ibm/fan
	else
		#u suprotnom stavljamo auto
		echo level auto > /proc/acpi/ibm/fan
	fi

#skripta sada spava 2 minuta pa okida ponovo
sleep 120
done
