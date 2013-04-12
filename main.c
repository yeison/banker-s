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
	
	int numberOfTasks = 0;
	int numberOfResourceTypes = 0;
	
	scanf("%d", &numberOfTasks);
	scanf("%d", &numberOfResourceTypes);
	/* The resourceTable keeps track of how much of each resource a particular task
    has requested. */
    int **resourceRequestTable = malloc(numberOfResourceTypes * sizeof(*resourceRequestTable));
    int *resourceRequestArray = malloc(numberOfResourceTypes*sizeof(int));

    // malloc the two-dimensional array
    if (resourceRequestTable)
    {
        for (int i = 0; i < numberOfResourceTypes; i++)
        {
            resourceRequestTable[i] = malloc(numberOfTasks * sizeof(*resourceRequestTable[i]));
        }
    }

    // The state array keeps track of the task's previous state
    int *previousState = malloc(numberOfTasks*sizeof(int));
    // The state array keeps track of the task's state
    int *currentState = malloc(numberOfTasks*sizeof(int));
    
	
	// Create resource arrays that track the amount of resource available of each type.
  	int *defaultResources = malloc(numberOfResourceTypes*sizeof(int));
	int *previousResources = malloc(numberOfResourceTypes*sizeof(int));
  	int *currentResources = malloc(numberOfResourceTypes*sizeof(int));
  	int *nextResources = malloc(numberOfResourceTypes*sizeof(int));
    
	for (int i = 0; i < numberOfResourceTypes; i++) {
        // Initialize resource array values
		scanf("%d", &defaultResources[i]);
        nextResources[i] = currentResources[i] = previousResources[i] = defaultResources[i];

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

    
    /* This function will read the entire input file and create queues of
       actions (activities) for each task */
	makeActivityQueues(taskTable, taskTableTails);	
    printQueues(taskTable, numberOfTasks);
   
    
    /*****************************************************************************/
    /* The meaty resource manager implementations */
    
    // Perform some initialization tasks
    activity **currentActivity = malloc((numberOfTasks+1)*sizeof(activity *));
    for (int i=0; i < numberOfTasks; i++) {
        // Copy task-queue head pointers
        currentActivity[i] = taskTable[i];
        // Set all states to 0
        previousState[i] = 0;
    }
    
    // Optimistic manager: process activities in each task queue
    int termCount = 0;
    int i = -1;
    int cycle = -1;
    printf("\nCycle: %d\n", cycle+1);
    printf("**************\n");
    
    // While all tasks have not been terminated
    while (termCount < numberOfTasks) {
        i++;
        cycle++;
        
        // Check if next cycle is a new cycle; then update table pointers and reset counters
        if (i%numberOfTasks == 0 && cycle != 0) {
            int *tempPtr = previousState;
            previousState = currentState;
            currentState = tempPtr;
            
            tempPtr = previousResources;
            previousResources = currentResources;
            currentResources = tempPtr;
            copyResourceArray(previousResources, currentResources, numberOfResourceTypes);
            copyResourceArray(previousResources, nextResources, numberOfResourceTypes);
            
            termCount = 0;
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
        
        activity *activity = currentActivity[i];
        switch (activity->type) {
            case INITIATE:
                // If state is 0 (not initiated) then initiate
                if (!previousState[i]) {
                    currentState[i] = INITIATE;
                }
                break;
            
            case REQUEST:
                if(previousState[i] >= INITIATE ){
                    // If resource of this type is available and sufficient for request
                    if (previousResources[activity->resourceType] >= activity->resourceAmount
                        && currentResources[activity->resourceType] >= activity->resourceAmount) {
                        currentResources[activity->resourceType] = currentResources[activity->resourceType] - activity->resourceAmount;
                        nextResources[activity->resourceType] = currentResources[activity->resourceType];
                        currentState[i] = GRANTED;
                        printf("\nGRANTED %d units of resource %d to task %d.\n\tRemaining: %d\n",
                               activity->resourceAmount,
                               activity->resourceType+1,
                               activity->taskNumber+1,
                               currentResources[activity->resourceType]);
                    } else {
                        printf("\nCould NOT grant %d units of resource %d to task %d: only %d available\n",
                               activity->resourceAmount,
                               activity->resourceType+1,
                               activity->taskNumber+1,
                               currentResources[activity->resourceType]);
                        checkDeadLock(activity->resourceType, resourceRequestTable, numberOfTasks);
                        currentState[i] = REJECTED;
                        
                    }
                } else {
                    perror("\nCannot perform REQUEST, previous state was not INITIATE.\n");
                    exit(2);
                }
                break;

            case RELEASE:
                if (previousState[i] > INITIATE) {
                    nextResources[activity->resourceType] = nextResources[activity->resourceType] + activity->resourceAmount;
                    if(nextResources[activity->resourceType] > defaultResources[activity->resourceType]){
                        nextResources[activity->resourceType] = defaultResources[activity->resourceType];
                    }
                    currentState[i] = RELEASE;
                    printf("\nTask %d RELEASED %d units of resource %d.\n\tAvailable Next Cycle: %d\n",
                           activity->taskNumber+1,
                           activity->resourceAmount,
                           activity->resourceType+1,
                           nextResources[activity->resourceType]);
                } else {
                    perror("\nCannot perform RELEASE, previous state was not REQUEST or RELEASE.\n");
                    exit(3);
                }

                break;
                
            default:
                break;
        }
        
        // On success move the pointer forward to the next activity in the linked list
        if (currentState[i] != REJECTED) {
            currentActivity[i] = currentActivity[i]->next;
        }
        
    }
	
	printf("\nDONE");
	
}

void copyResourceArray(int *from, int *to, int size){
    
    for (int i = 0; i < size; i++) {
        to[i] = from[i];
    }    

}

bool checkDeadLock(int resourceType, int **resourceRequestTable, int numberOfTasks){
    int firstWaitingTask;
    
    for (int j = 0; j < numberOfTasks; j++) {
        if(resourceRequestTable[resourceType][j]){
            firstWaitingTask = resourceRequestTable[resourceType][j];
        }
    }
    

    
}
