/*
 *  activity.c
 *  banker
 *  Created by Yeison Rodriguez on 12/14/09.
 *
 */
#include "activity.h"

/*The best implementation here would be to turn the function below into a 
 makeNode function (make activity).  Then make an activity for every properly 
 formatted line, and insert that activity into a linked list.*/
activity* makeActivityNode(int taskNumber){
	char type[MAX_STRING_LENGTH] = "";
	activity *currentActivity = malloc(sizeof(activity));
    // Set the task-number
    currentActivity->taskNumber = taskNumber;
    // Read delay from stdin
	scanf("%d", &currentActivity->delay);
    // Read activity type
	scanf("%s", type);
	currentActivity->type = getActivityType(type);
    // Read resource type
	scanf("%d", &currentActivity->resourceType);
    // Amount of resource for the activity (e.g. resource released/requested) 
	scanf("%d", &currentActivity->resourceAmount);
	return currentActivity;
}


/*The function below creates a linked list of activityNodes, and returns the 
 head of that list.  This way we can have various parallel lists, each one 
 corresponding to a particular task number.  During each cycle we can pull the 
 next request from each list.*/
void makeActivityList(activity *taskTable[], activity *taskTableTails[]){
	int currentTask;
	int headTask;
	activity *currentActivityPtr;
	activity *headActivityPtr;
	scanf("%d", &headTask);
    // Read the task-number of the next activity and save it as currentTask
	currentTask = headTask;
	/* If the task has already been seen, insert the new activity nodes at the 
	end of its corresponding list (queue).*/
	if (taskTableTails[currentTask]){
		currentActivityPtr = taskTableTails[currentTask];
		headActivityPtr = taskTable[currentTask];
	}
	/*Otherwise, the task has not been seen.  Therefore, we start a new queue 
	for the new task. */
	else {
		headActivityPtr = makeActivityNode(currentTask);
		currentActivityPtr = headActivityPtr;
		scanf("%d", &currentTask); // Read the task-number of the next activity
	}
	// While statement will finish when the taskNumber changes.
	while (headTask == currentTask) {
		currentActivityPtr->next = makeActivityNode(currentTask);
		scanf("%d", &currentTask);
	}
	// Identify the last element by pointing its next to NULL.
	// (*currentActivityPtr).next = NULL; // This should happen auto-magically
	// Save the tail of the list in the tail's table.
	taskTableTails[headTask] = currentActivityPtr;
	// Return the head of the list.
	taskTable[headTask] = headActivityPtr;
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