banker: main.o activity.o
	gcc -o banker main.o activity.o

main.o: main.c main.h
	gcc -std=c99 -c main.c

activity.o: activity.c activity.h
	gcc -std=c99 -c activity.c


clean:
	rm banker main.o activity.o
