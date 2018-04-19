
all: main.c lib/*.h lib/view/*.h
	gcc -Wall -ggdb main.c lib/*.h lib/view/*.h -lncurses -lmenu -o ccards
