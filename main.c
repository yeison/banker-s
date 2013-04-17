#include "main.h"
#include "debugging.h"
#include "limits.h"

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
	
	int numberOfTasks = 0;
	int numberOfResourceTypes = 0;
	
	scanf("%d", &numberOfTasks);
	scanf("%d", &numberOfResourceTypes);
	/* The resourceLockTable keeps track of how much of each resource a particular task
    has been granted. */    
    int **resourceLockTable = malloc2dIntArray(numberOfResourceTypes, numberOfTasks);

    // The state array keeps track of the task's previous state
    int **previousState = malloc2dIntArray(numberOfResourceTypes, numberOfTasks);
    // The state array keeps track of the task's state
    int **currentState = malloc2dIntArray(numberOfResourceTypes, numberOfTasks);


	// Create resource arrays that track the amount of resource available of each type.
  	int *defaultResources = malloc(numberOfResourceTypes*sizeof(int));
	int *initialResources = malloc(numberOfResourceTypes*sizeof(int));
  	int *currentResources = malloc(numberOfResourceTypes*sizeof(int));
  	int *nextResources = malloc(numberOfResourceTypes*sizeof(int));

    // Initialize resource array values
	for (int i = 0; i < numberOfResourceTypes; i++) {
		scanf("%d", &defaultResources[i]);
        nextResources[i] = currentResources[i] = initialResources[i] = defaultResources[i];
	}

	/*** Test the first line of input ***/
	printf(" Number of Tasks: %d\n", numberOfTasks);
	printf(" Number of Resource Types: %d\n", numberOfResourceTypes);
	for (int i = 0; i < numberOfResourceTypes; i++) {
		printf(" Units of resource-type %d: %d\n", i+1, currentResources[i]);
	}
	
	/* taskTable will contain pointers to the head of a queue of activities.
       The taskTable index corresponds to the task number (zero-based counting). 
       An additional bit of buffer space is being allocated. */
	activity **taskTable = malloc((numberOfTasks+1)*sizeof(activity *));
	activity **taskTableTails = malloc((numberOfTasks+1)*sizeof(activity *));

    /* This function reads the entire input file and creates queues of
       actions (activities) for each task */
	makeActivityQueues(taskTable, taskTableTails);	
    printQueues(taskTable, numberOfTasks);

    /*****************************************************************************/
    /* The meaty resource manager implementation */

    // Perform some initialization tasks
    activity **currentActivity = malloc((numberOfTasks+1)*sizeof(activity *));
    for (int i=0; i < numberOfTasks; i++) {
        // Copy task-queue head pointers
        currentActivity[i] = taskTable[i];
    }

    // Optimistic manager: process activities in each task queue
    int termCount = 0;
    int waitingCount = 0;
    int i = -1;
    int cycle = -1;
    int *minRequest = malloc(numberOfResourceTypes*sizeof(int));
    for (int i = 0; i < numberOfResourceTypes; i++) {
        minRequest[i] = INT_MAX;
    }
    printf("\nCycle: %d\n", cycle+1);
    printf("**************\n");

    // While all tasks have not been terminated
    while (termCount < numberOfTasks) {
        i++;
        cycle++;
        
        // Check if next cycle is a new cycle; then update table pointers and reset counters
        if (i%numberOfTasks == 0 && cycle != 0) {            
            
            int **tempStatePtr = previousState;
            previousState = currentState;
            currentState = tempStatePtr;
            // Prime the new currentState array with old currentState data
            copy2dArray(previousState, currentState, numberOfResourceTypes, numberOfTasks);
            
            int *tempResourcePtr = initialResources;
            initialResources = nextResources;
            currentResources = nextResources;
            nextResources = tempResourcePtr;
            copyArray(initialResources, nextResources, numberOfResourceTypes);
            
            // If all tasks are waiting, we have a deadlock
            if (waitingCount >= numberOfTasks-termCount) {
                resolveDeadlock(resourceLockTable, numberOfTasks, numberOfResourceTypes, currentResources, currentActivity, minRequest);
            }

            termCount = 0;
            waitingCount = 0;
            i = 0;
            printf("\nCycle: %d\n", (cycle/numberOfTasks));
            printf("**************\n");
        }

        // If there are no more activities for this task, continue to next task
        if(currentActivity[i] == NULL){
            // Break if we have NULLs equal to numberOfTasks
            if(numberOfTasks == ++termCount){
                break;
            }
            continue;
        }

        activity *activityN = currentActivity[i];

        // If the activity has a delay, skip it until the delay is exhausted
        if(activityN->type != TERMINATE && activityN->type != INITIATE && activityN->delay > 0){
            activityN->delay--;
            printf("\nTask %d skipping delayed activity: %s\n\tRemaining delay: %d\n", activityN->taskNumber+1, getActivityType(activityN->type), activityN->delay);
            currentState[activityN->resourceType][i] = DELAYED;
            continue;
        }

        switch (activityN->type) {
            case INITIATE:
                // If state is 0 (not initiated) then initiate
                if (!previousState[activityN->resourceType][i]) {
                    currentState[activityN->resourceType][i] = INITIATE;
                }
                break;

            case REQUEST:
                if(previousState[activityN->resourceType][i] >= INITIATE){
                    // If resource of this type is available and sufficient for request
                    if(initialResources[activityN->resourceType] >= activityN->resourceAmount
                    && currentResources[activityN->resourceType] >= activityN->resourceAmount) {                        
                        currentResources[activityN->resourceType] = currentResources[activityN->resourceType] - activityN->resourceAmount;
                        nextResources[activityN->resourceType] = currentResources[activityN->resourceType];
                        currentState[activityN->resourceType][i] = GRANTED;
                        printf("\nGRANTED %d unit(s) of resource %d to task %d.\n\tRemaining: %d\n",
                               activityN->resourceAmount,
                               activityN->resourceType+1,
                               activityN->taskNumber+1,
                               currentResources[activityN->resourceType]);
                        resourceLockTable[activityN->resourceType][activityN->taskNumber] = activityN->resourceAmount;
                    } else {
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: only %d available\n",
                               activityN->resourceAmount,
                               activityN->resourceType+1,
                               activityN->taskNumber+1,
                               currentResources[activityN->resourceType]);
                        currentState[activityN->resourceType][i] = WAITING;
                        waitingCount++;
                        minRequest[activityN->resourceType] = min(activityN->resourceAmount, minRequest[activityN->resourceType]);
                    }
                } else {
                    perror("\nCannot perform REQUEST, previous state was not INITIATE.\n");
                    exit(2);
                }
                break;

            case RELEASE:
                if (previousState[activityN->resourceType][i] > INITIATE) {
                    nextResources[activityN->resourceType] = nextResources[activityN->resourceType] + activityN->resourceAmount;
                    if(nextResources[activityN->resourceType] > defaultResources[activityN->resourceType]){
                        nextResources[activityN->resourceType] = defaultResources[activityN->resourceType];
                    }
                    currentState[activityN->resourceType][i] = RELEASE;
                    printf("\nTask %d RELEASED %d unit(s) of resource %d.\n\tUnits available next cycle: %d\n",
                           activityN->taskNumber+1,
                           activityN->resourceAmount,
                           activityN->resourceType+1,
                           nextResources[activityN->resourceType]);
                    resourceLockTable[activityN->resourceType][activityN->taskNumber] = activityN->resourceAmount;
                } else {
                    perror("\nCannot perform RELEASE, previous state was not REQUEST or RELEASE.\n");
                    exit(3);
                }

                break;

            case TERMINATE:
                termCount++;
                break;

            default:
                break;
        }

        // On success move the pointer forward to the next activity in the linked list
        if (activityN->type != TERMINATE  && currentState[activityN->resourceType][i] != WAITING) {
            currentActivity[i] = currentActivity[i]->next;
        }

    }

	printf("\nDONE");

}


bool resolveDeadlock(int **resourceLockTable, int numberOfTasks, int numberOfResourceTypes, int *currentResources, activity **currentActivity, int *minRequest){

    printf("\nResolving Deadlock\n");
    bool success = false;

    for (int i = 0; i < numberOfTasks; i++) {
        for (int j = 0; j < numberOfResourceTypes; j++) {
            if(resourceLockTable[j][i]){

                printf("\nAborting task %d\n", i+1);
                currentActivity[i]->type = TERMINATE;

                while (j < numberOfResourceTypes) {
                    int resourceAmount = resourceLockTable[j][i];

                    currentResources[j] += resourceAmount;
                    printf("\tTask %d forced to release %d unit(s) of resource %d: %d units available next cycle\n", i+1, resourceAmount, j+1, currentResources[j]);

                    if (currentResources[j] >= minRequest[j]) {
                        success = true;
                    }

                    resourceLockTable[j][i] = 0;
                    j++;

                }
                if(success){
                    return success;
                }
            }
        }
    }

    return success;

}

int **malloc2dIntArray(int x, int y){
    int **twoDArray = malloc(x * sizeof(*twoDArray));
    
    for (int i=0; i < x; i++) {
        twoDArray[i] = malloc(y * sizeof(*twoDArray[i]));
    }
    
    return twoDArray;
}

void copyArray(int *from, int *to, int size){
    
    for (int i = 0; i < size; i++) {
        to[i] = from[i];
    }
    
}

void copy2dArray(int **from, int **to, int x, int y){
    for (int i=0; i < x; i++) {
        copyArray(from[i], to[i], y);
    }
}