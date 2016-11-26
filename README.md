# nosdk8266

This is a working ESP8266/ESP8285 minimial, non-SDK application.

It uses the ROM functions to deal with I/O, interrupts, etc. 

It operates at 80 MHz, sets up an interrupt and prints out a message periodically.


...unless you're running in PICO66 mode (see Makefile)

In this mode, no setup is performed of serial (printfs are nulled out), and no clock changing is performed.  It has a delay function specifically tailored to assuming a 52 MHz clock.  The PICO66 mode is for the hackaday 1kB challenge: https://hackaday.io/contest/18215-the-1kb-challenge

