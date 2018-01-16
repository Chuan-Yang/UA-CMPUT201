# name:               Chuan Yang
# ONE Card number:    1421992
# Unix id:            chuan1
# lecture section:    A1
# lab section:        D05
# TA's name:          Juehui Fan
#

draw1b: draw1b.o
	gcc -Wall -std=c99 -o draw1b draw1b.o -lm

draw1b.o: draw1b.c draw1b.h
	gcc -Wall -std=c99 -c draw1b.c

clean:
	-rm -f *.o draw1b core
