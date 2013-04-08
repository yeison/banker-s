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
	/* The resourceTable keeps track of how much of each resource a particular task
    has requested. */
	int resourceRequestTable[numberOfResourceTypes][numberOfTasks];
    // The state array keeps track of the task's previous state
    int previousState[numberOfTasks];
    // The state array keeps track of the task's state
    int currentState[numberOfTasks];
	
	// Create a resource array that holds the amount of resource available of each type.
	int resourceArray[numberOfResourceTypes];	
    
	for (int i = 0; i < numberOfResourceTypes; i++) {
        // Initialize resource array values
		scanf("%d", &resourceArray[i]);
	}

	/*** Test the first line of input ***/
	printf(" Number of Tasks: %d\n", numberOfTasks);
	printf(" Number of Resource Types: %d\n", numberOfResourceTypes);
	for (int i = 0; i < numberOfResourceTypes; i++) {
		printf(" Units of resource-type %d: %d\n", i+1, resourceArray[i]);
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
    
    
    // Copy head pointers
    activity **currentActivity = malloc((numberOfTasks+1)*sizeof(activity *));
    for (int i=0; i < numberOfTasks; i++) {
        currentActivity[i] = taskTable[i];
    }
    
    // Process activities in each task queue
    for (int i=0; i < numberOfTasks; i++) {
        
        // If there are no more activities, continue to next task
        if(currentActivity == NULL){
            continue;
        }
        
        switch (currentActivity[i]->type) {
            case INITIATE:
                // If state is 0 (not initiated) then initiate
                if (!previousState[currentActivity[i]->taskNumber]) {
                    currentState[currentActivity[i]->taskNumber] = INITIATE;
                }
                break;
            
            case REQUEST:                
                break;
                
                
            default:
                break;
        }
        
        // Move the pointer forward to the next activity in the linked list
        currentActivity[i] = currentActivity[i]->next;
    }
	
	printf("\nDONE");
	
}
