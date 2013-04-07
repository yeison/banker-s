/*
 *  activity.h
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/14/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */
#include "main.h"

/* An activity is a struct, which contains all of the information from one typical "line" of input.  It also contains a next pointer, so that activity nodes may be arranged in a linked list.  The linked list will be used as a queue.  This way we can maintain task activities in the appropriate order.*/

typedef struct activity activity;

struct activity {
	int taskNumber;
	int delay;
	int type;
	int resourceType;
	int resourceAmount;
	activity *next;
};

activity* makeActivityNode();
void makeActivityList(activity *taskTable[], activity *taskTableTails[]);
int getActivityType(char *type);