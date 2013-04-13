/*
 *  activity.h
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/14/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */

/* An activity is a struct, which contains all of the information from one typical "line" of input.  It also contains a next pointer, so that activity nodes may be arranged in a linked list.  The linked list will be used as a queue.  This way we can maintain task activities in the appropriate order.*/

typedef struct activity activity;

struct activity {
	int taskNumber;
	int delay;
	int type;
	int resourceType;
	int resourceAmount;
    int currentState;
	activity *next;
};

// Create an activity node (an action to be performed on a task)
activity* makeActivityNode();
// Create queues of actions(activities) for each task
void makeActivityQueues(activity *taskTable[], activity *taskTableTails[]);
// Retrieve the integer value for the string type
int getActivityTypeValue(char *type);
// Retrieve string-value for the activity type
char* getActivityType(int activityTypeValue);

#define TERMINATE 1
#define INITIATE 2
#define REQUEST 3
#define RELEASE 4
#define GRANTED 6
#define WAITING 7
#define MAX_STRING_LENGTH 10