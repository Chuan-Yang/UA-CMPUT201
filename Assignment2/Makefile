# name:               Chuan Yang
# ONE Card number:    1421992
# Unix id:            chuan1
# lecture section:    A1
# lab section:        D05
# TA's name:          Juehui Fan
#

draw2: draw2.o
	gcc -Wall -std=c99 -o draw2 draw2.o -lm

draw2.o: draw2.c draw2.h
	gcc -Wall -std=c99 -c draw2.c

clean:
	-rm -f *.o draw2 core
