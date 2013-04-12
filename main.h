/*
 *  main.h
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/8/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void copyResourceArray(int *from, int *to, int size);

bool checkDeadLock(int resourceType, int **resourceRequestTable, int numberOfTasks);