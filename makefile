banker: main.o activity.o debugging.o
	gcc -o banker main.o activity.o debugging.o

main.o: main.c main.h
	gcc -std=c99 -c main.c

activity.o: activity.c activity.h
	gcc -std=c99 -c activity.c

debugging.o: debugging.c debugging.h
	gcc -std=c99 -c debugging.c

clean:
	rm banker main.o activity.o debugging.o
