#include "main.h"

int main (int argc, const char * argv[]) {
	if (argc < 2) {
		//Explain proper usage of the program at the command line.
		printf("\tUsage: banker <name of input file> \n\n");
		exit(0);
	}
	
	//Redirect standard input stream to the file provided as the first argument.
	stdin = fopen(argv[1], "r");
	
	//Check to see if fopen() was successful
	if (stdin == NULL) {
		//If not, then print this message to the standard error output stream.
		perror("Error: File not found. Make sure the file exists. If the file name contains spaces, then surround the whole name with quotes.\nError");
		//and exit with an error code of 1
		exit(1);
	}
	
	/*****************************************************************************/
	/*Begin Implementation*/
	
	int numberOfTasks;
	int numberOfResourceTypes;
	
	scanf("%d", &numberOfTasks);
	scanf("%d", &numberOfResourceTypes);
	//The resourceTable indicates how much of each resource a particular task is holding.
	int resourceTable[numberOfResourceTypes][numberOfTasks];

	
	//Create a resource array that holds the amount of resources of each type.
	int resourceArray[numberOfResourceTypes+1];
	
	for (int i = 1; i <= numberOfResourceTypes; i++) {
		scanf("%d", &resourceArray[i]);
	}

	/*** Test the first line of input ***/
	printf(" Number of Tasks: %d\n", numberOfTasks);
	printf(" Number of Resource Types: %d\n", numberOfResourceTypes);
	for (int i = 1; i <= numberOfResourceTypes; i++) {
		printf(" Amount of resource %d: %d\n", i, resourceArray[i]);
	}
	
	//	int currentTask = (*headActivity).taskNumber;
	//taskTable will probably contain pointers to the head of a list of activities
	activity *taskTable[numberOfTasks+1];
	activity *taskTableTails[numberOfTasks+1];
	for (int i = 1; i <= numberOfTasks; i++)
		taskTableTails[i] = 0;
	taskTable[0] = makeActivityList(taskTable, taskTableTails);
	printf("DONE");
	
}



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