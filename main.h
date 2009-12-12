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
#include <string.h>
#include "simclist.h"

#define initiate 0
#define request 1
#define release 2
#define terminate 3
#define MAX_STRING_LENGTH 10

typedef struct activity activity;

struct activity {
	int taskNumber;
	int delay;
	char type[MAX_STRING_LENGTH];
	int resourceType;
	int resourceAmount;
	activity *next;
};

activity makeActivityNode();
activity *makeActivityList();