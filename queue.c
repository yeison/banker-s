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

void push_front( int val, queue *queue ){
    queue_int *new = malloc(sizeof(queue_int));
    
    new->value = val;
    
    if (queue->tail != NULL) {
        new->previous = queue->tail;        
        queue->tail->next = new;
    } else {
        queue->head = queue->tail = new;
    }
    
    queue->tail = new;
}

// Returns the head value (LIFO)
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
        queue->head->previous = NULL;
    }
    
//    free(head);    
    
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
        queue->tail->next = NULL;
    }
    
//    free(tail);
    
    return value;    
}

bool is_empty( queue *queue ){
    if (queue->head == NULL && queue->tail == NULL) {
        return true;
    }
    
    return false;
}