# Makefile

all:
	gcc lights_off.c -o lights_off -lncurses
	
clean: 
	rm -rf lights_off
