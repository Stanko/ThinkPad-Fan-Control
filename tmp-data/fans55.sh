#!/bin/bash

#podesi vremena i temperature kako ti odgovaraju
START="0"
#mrtva petlja u kojoj se sve desava
while true; do

#cupam trenutnu temperaturu procesora
TEMP=`cat /proc/acpi/ibm/thermal | awk '{print $2}'`
#TEMP=`cat ~/thermal | awk '{print $2}'`

	#ako je temperatura preko 60, a fanovi nisu na fullu startujem ih
  if [ "$TEMP" -ge "60" ] && [ "$START" -eq "0" ]; then
		echo "Temperatura je presla 60 stepeni. Startujem fanove."
		date '+.:: %H:%M:%S ::.'
		echo "-------------------------------------------------------------"
		START="1"
		echo level disengaged > /proc/acpi/ibm/fan
	fi

	#ako je temperatura ispod 55 a fanovi su na fullu vracam ih na auto
	if [ "$TEMP" -lt "55" ] && [ "$START" -eq "1" ]; then
		echo "Temperatura je pala ispod 55 stepeni. Vracam fanove na auto. "
		date '+.:: %H:%M:%S ::.'
		echo "-------------------------------------------------------------"
		START="0"
		echo level auto > /proc/acpi/ibm/fan
	fi
		
#skripta sada spava 2 minuta pa okida ponovo
sleep 120
done

