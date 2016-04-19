ThinkPad Fan Control
============================
Version 0.5

Unfortunately this project is NOT in the active development
---------------------

I don't own a ThinkPad laptop for years now, and I don't plan to continue development on this. 
Feel free to check forks fine people made or issue a pull request. Thank you for your understanding.


INTRODUCTION
---------------------
This is program for controlling fans speed on IBM/Lenovo ThinkPads. It is written
for Linux only. This program is written in C, using GTK GUI.
 
You are required to have the Linux kernel with 'thinkpad-acpi' patch.
You must also enable manual control for your fans. For Linux 2.6.22 and above,
you must add 'fan_control=1' as a module parameter to 'thinkpad-acpi'.
For example, in Debian Lenny (and Ubuntu 8.04), you must add the following
to "/etc/modprobe.d/options":
        options thinkpad_acpi fan_control=1
In Ubuntu 9.10 you need to add this line to file "/etc/modprobe.d/alsa-base.conf"
 
Having done so, reboot. Now you'll be able to use this program easily.
 
I wrote this for my own personal use, and thought that it would be a good idea
to release it to the world, and hope that it will be useful to someone!
Feel free to send comments, bug reports or a thanks to the e-mail above.


COMPILATION
---------------------
You will need GTK development libraries. The Makefile that is available can
be used like:

    make

There is also a pre-compiled binary available for i386, 32-bit machines.

START-UP
---------------------------------
You must run this program as root, because only root can change the speed of fans.
I personally recommend using "tpfc_start.sh", which will run ThinkPad Fan Control
with gksu under administrator privileges.
 
I also made "tpfc_start-up.sh", which I added to my GNOME start-up list.
Using it will let you run the program when you log on. All you need to do is to
change two lines in it, path in which ThinkPad Fan Control is located, and sleep
time - the amount of time the script will wait until launching TPFC.


USAGE
--------------------
The program has two modes - automatic and manual. The manual mode can't be easier
to use - just choose the speed and click the "Change speed" button.

The automatic mode is a bit more complicated. There are four options you can change:

* Sleep time - how often the program will check for the CPUtemperature (default - 120s).
* Critical temperature - the program will speed up the fans when this temperature is reached (default - 55C).
* Safe temperature - the program will switch the fans to normal when the temperature is lower than this (default - 50C)
* Fan level speed - the speed of fans when the CPU reaches the critical temperature (default - 7).
 
If you would like to customise these options, just click the 
"Change options button"


I hope you enjoy using this :) :D

