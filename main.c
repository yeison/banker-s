#include "main.h"
#include "activity.h"
#include "debugging.h"

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
	
	//Create a resource array that holds the number of resources present for each type.
	int resourceArray[numberOfResourceTypes+1];	
    
	for (int i = 1; i <= numberOfResourceTypes; i++) {
		scanf("%d", &resourceArray[i]);
	}

	/*** Test the first line of input ***/
	printf(" Number of Tasks: %d\n", numberOfTasks);
	printf(" Number of Resource Types: %d\n", numberOfResourceTypes);
	for (int i = 1; i <= numberOfResourceTypes; i++) {
		printf(" Units for resource-type %d: %d\n", i, resourceArray[i]);
	}
	
	// taskTable will probably contain pointers to the head of a list of activities
	activity **taskTable = malloc((numberOfTasks+1)*sizeof(activity *));
	activity **taskTableTails = malloc((numberOfTasks+1)*sizeof(activity *));

    
    /* This function will read the entire input file and create queues of
       actions for each task */
	makeActivityList(taskTable, taskTableTails);	
    printQueues(taskTable, numberOfTasks);
	
	printf("\nDONE");
	
}
