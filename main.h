/*
 *  main.h
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/8/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */

#include "activity.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


void copyResourceArray(int *from, int *to, int size);

// Aborts lowest-numbered deadlocked process, and releases claimed resources
bool resolveDeadlock(int **resourceLockTable, int numberOfTasks, int numberOfResourceTypes, int *currentResources, activity **currentActivity);