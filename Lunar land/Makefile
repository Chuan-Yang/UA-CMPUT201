# name:               Chuan Yang
# ONE Card number:    1421992
# Unix id:            chuan1
# lecture section:    A1
# lab section:        D05
# TA's name:          Juehui Fan
#

lander: lander.o
	gcc -Wall -std=c99 -o lander lander.o -lm -lncurses

lander.o: lander.c lander.h
	gcc -Wall -std=c99 -c lander.c 

clean:
	-rm -f *.o lander core
