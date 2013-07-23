//  queue.c
//  bankers
//
//  Created by Yeison Rodriguez on 6/17/13.
//
//  Actually, this is a double-ended queue

#include "queue.h"


void push( int val, queue *queue ){
    queue_int *new = malloc(sizeof(queue_int));
    
    new->value = val;
    
    if (queue->head != NULL) {
        new->next = queue->head;
        queue->head->previous = new;
    } else {
        queue->head = queue->tail = new;
    }

    queue->head = new;
}

// Returns the head (LIFO)
int pop_back( queue *queue ){
    
    if(queue->head == NULL){
        return NULL;
    }
    
    int value = queue->head->value;
    
    queue_int *head = queue->head;
    
    if (queue->head == queue->tail) {
        queue->head = queue->tail = NULL;
    } else {
        queue->head = head->next;
    }
    
    free(head);    
    
    return value;
}

// Returns the tail (FIFO)
int pop_front( queue *queue ){
    
    if(queue->tail == NULL){
        return NULL;
    }
    
    int value = queue->tail->value;    
    queue_int *tail = queue->tail;
    
    if (queue->head == queue->tail) {
        queue->head = queue->tail = NULL;
    } else {
        queue->tail = tail->previous;
    }
    
    free(tail);
    
    return value;
    
}
