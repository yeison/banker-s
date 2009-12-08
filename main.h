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

#define initiate 0
#define request 1
#define release 2
#define terminate 3

typedef struct activity activity;

struct activity {
	char taskNumber;
	char delay;
	char type;
	char resourceType;
	char resourceAmount;
	activity *nextActivity;
};

void buildActivityList(activity currentActivity, activity *taskTable[]);