/*
 *  activity.h
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/14/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */
#include "main.h"

typedef struct activity activity;

struct activity {
	int taskNumber;
	int delay;
	char type;
	int resourceType;
	int resourceAmount;
	activity *next;
};

activity makeActivityNode();
activity *makeActivityList(activity *taskTable[], activity *taskTableTails[]);
char getActivityType(char *type);