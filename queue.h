//
//  queue.h
//  bankers
//
//  Created by Yeison Rodriguez on 6/17/13.
//
//
//  Actually, this is a double-ended queue
#include <stdbool.h>


#ifndef bankers_queue_h
#include <stdlib.h>
#define bankers_queue_h
#endif


typedef struct queue_int queue_int;
struct queue_int {
    int value;    
	queue_int *next;
  	queue_int *previous;
};

typedef struct queue queue;
struct queue{
    queue_int *head;
    queue_int *tail;
};


void push( int i, queue *queue );
int pop_front( queue *queue );
int pop_back( queue *queue );
bool is_empty( queue *queue );