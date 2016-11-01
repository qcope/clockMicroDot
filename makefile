clock:	clock.o microDotDisplay.o
	gcc clock.o microDotDisplay.o -o clock -lpigpio -lrt

clock.o: clock.c microDotDisplay.h
	gcc -c clock.c -o clock.o

microDotDisplay.o: microDotDisplay.c microDotDisplay.h	
	gcc -c -Wall -pthread microDotDisplay.c -o microDotDisplay.o
