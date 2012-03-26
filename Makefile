PROGRAM = tp_fan_control
CC      = gcc
CFLAGS  = -g -Wall
GTKLIBS = `pkg-config gtk+-2.0 libglade-2.0 --cflags --libs`
SRCPATH = src/
BINPATH = data/

$(PROGRAM): $(SRCPATH)$(PROGRAM).c
	$(CC) $(CFLAGS) -o $(BINPATH)$(PROGRAM) $(SRCPATH)$(PROGRAM).c $(GTKLIBS)

.PHONY: beauty clean dist

beauty:
	-indent $(PROGRAM).c
	-rm *~ *BAK

clean:
	-rm *.o $(PROGRAM) *core

dist: beauty clean
	-tar -chvz -C .. -f ../$(PROGRAM).tar.gz $(PROGRAM)

