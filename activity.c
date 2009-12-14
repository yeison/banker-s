/*
 *  activity.c
 *  banker
 *
 *  Created by Yeison Rodriguez on 12/14/09.
 *  Copyright 2009 New York University. All rights reserved.
 *
 */
#include "activity.h"

/*The best implementation here would be to turn the function below into a makeNode function (make activity).  Then make an activity for every properly formatted line,, and insert that activity into a linked list.*/
activity makeActivityNode(){
	char type[MAX_STRING_LENGTH];
	activity currentActivity;
	scanf("%d", &currentActivity.delay);
	scanf("%s", type);
	currentActivity.type = getActivityType(type);
	scanf("%d", &currentActivity.resourceType);
	scanf("%d", &currentActivity.resourceAmount);
	return currentActivity;
}


/*The function below creates a linked list of activityNodes.  This way we can have various parallel lists, each one corresponding to a particular task number.  During each cycle we can pull the next request from each list.*/
activity *makeActivityList(activity *taskTable[], activity *taskTableTails[]){
	int currentTask;
	activity *currentActivity;
	activity *headActivity;
	scanf("%d", &currentTask); // Read the task of the next activity
	// If the task has already been seen, insert the new activity nodes at the end of its corresponding list (queue).
	if (taskTableTails[currentTask]){
		currentActivity = taskTableTails[currentTask];
		headActivity = taskTable[currentTask];
	}
	// Otherwise, the task has not been seen.  Therefore, we start a new queue for the new task.
	else {
		headActivity = malloc(sizeof(activity));
		*headActivity = makeActivityNode();
		(*headActivity).taskNumber = currentTask;
		currentActivity = headActivity;
		scanf("%d", &currentTask); // Read the task of the next activity
	}
	//While statement will finish when the taskNumber changes.
	while ((*headActivity).taskNumber == currentTask) {
		(*currentActivity).next = malloc(sizeof(activity));
		currentActivity = (*currentActivity).next;
		*currentActivity = makeActivityNode();
		(*currentActivity).taskNumber = currentTask;
		scanf("%d", &currentTask);
	}
	//Save the tail of the list in the tail's table.
	taskTableTails[currentTask] = currentActivity;
	//Return the head of the list.
	return headActivity;
}

//Convert the activity string into a constant corresponding to the type indicated.
char getActivityType(char *type){
	if(!strcmp(type, "initiate"))
		return initiate;
	else if(!strcmp(type, "request"))
		return request;
	else if(!strcmp(type, "release"))
		return release;
	else if(!strcmp(type, "terminate"))
		return terminate;
	else {
		perror("Activity type requested is unknown.  Please use proper formatting.");
		exit(2);
	}
}