//
//  utility.h
//  bankers
//
//  Created by Yeison Rodriguez on 4/17/13.
//
//

#ifndef bankers_utility_h
#define bankers_utility_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "limits.h"
#include "activity.h"

void copyArray(int *from, int *to, int size);

// Abort the given task, and release its resources
void abortTask(int taskNumber, int **resourceLockTable, int numberOfResourceTypes, int *currentResources, activity **currentActivity);

// Aborts lowest-numbered deadlocked process, and releases resources
bool resolveDeadlock(int **resourceLockTable, int numberOfTasks, int numberOfResourceTypes, int *currentResources, activity **currentActivity, int *minRequest, int *taskTimeTable);

// Allocates space for a two-dimensional array
int **malloc2dIntArray(int x, int y);

void copy2dArray(int **from, int **to, int x, int y);

// Print the output of these runs
void printOutput(int *taskTimeTable, int *taskWaitingTable, int numberOfTasks);


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


#endif