banker: main.o activity.o debugging.o utility.o dijkstras.o optimistic.o queue.o
	gcc -o banker main.o activity.o debugging.o utility.o dijkstras.o optimistic.o queue.o

main.o: main.c main.h
	gcc -std=c99 -c main.c

activity.o: activity.c activity.h
	gcc -std=c99 -c activity.c

debugging.o: debugging.c debugging.h
	gcc -std=c99 -c debugging.c

utility.o: utility.c utility.h
	gcc -std=c99 -c utility.c

dijkstras.o: dijkstras.c dijkstras.h
	gcc -std=c99 -c dijkstras.c
	
optimistic.o: optimistic.c optimistic.h
	gcc -std=c99 -c optimistic.c

queue.o: queue.c queue.h
	gcc -std=c99 -c queue.c

clean:
	rm banker main.o activity.o debugging.o utility.o dijkstras.o optimistic.o queue.o
