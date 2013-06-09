//
//  dijkstras.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/17/13.
//
//

#include "dijkstras.h"

void runBankers(){
    
	/*****************************************************************************/
	/*Begin Implementation*/
	
	int numberOfTasks = 0;
	int numberOfResourceTypes = 0;
	
	scanf("%d", &numberOfTasks);
	scanf("%d", &numberOfResourceTypes);

    // Tables to store output data
    int *taskTimeTable = malloc(numberOfTasks*sizeof(int));
    int *taskWaitingTable = malloc(numberOfTasks*sizeof(int));

	/* The resourceLockTable keeps track of how much of each resource a particular task
     has been granted. */
    int **resourceLockTable = malloc2dIntArray(numberOfResourceTypes, numberOfTasks);
    
    // Keeps track of how many resources each task claims it will use
    int **resourceClaimTable = malloc2dIntArray(numberOfResourceTypes, numberOfTasks);
    
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
  	printf("================= BANKER'S ==================\n\n");
	printf("Number of Tasks: %d\n", numberOfTasks);
	printf("Number of Resource Types: %d\n", numberOfResourceTypes);
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
                resolveDeadlock(resourceLockTable, numberOfTasks, numberOfResourceTypes, currentResources, currentActivity, minRequest, taskTimeTable);
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
                // If claim exceeds total resource, then abort the task
                if(activityN->resourceAmount > defaultResources[activityN->resourceType]){
                    
                    printf("\nCould NOT claim %d unit(s) of resource %d for task %d: \n\tClaim EXCEEDS total available resources: %d\n",
                           activityN->resourceAmount,
                           activityN->resourceType+1,
                           activityN->taskNumber+1,
                           defaultResources[activityN->resourceType]);
                    abortTask(activityN->taskNumber, resourceLockTable, numberOfResourceTypes, currentResources, currentActivity);
                    currentState[activityN->resourceType][i] = TERMINATE;
                    break;
                }
                // If state is 0 (not initiated) then initiate
                if (!previousState[activityN->resourceType][i]) {
                    currentState[activityN->resourceType][i] = INITIATE;
                    // Retrieve and store the claim amount
                    resourceClaimTable[activityN->resourceType][activityN->taskNumber] = activityN->resourceAmount;
                }
                break;
                
            case REQUEST:
                if(previousState[activityN->resourceType][i] >= INITIATE){
 
                    // If request exceeds initial claims, then abort the task
                    if(activityN->resourceAmount > resourceClaimTable[activityN->resourceType][activityN->taskNumber]){
                    
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: \n\tEXCEEDS initial claim of %d\n",
                               activityN->resourceAmount,
                               activityN->resourceType+1,
                               activityN->taskNumber+1,
                               resourceClaimTable[activityN->resourceType][activityN->taskNumber]);
                        abortTask(activityN->taskNumber, resourceLockTable, numberOfResourceTypes, currentResources, currentActivity);
                        currentState[activityN->resourceType][i] = TERMINATE;
                        break;
                    }
                    
                    // If initial claim exceeds available resources and resources have not already been allocated, deny request
                    if(resourceClaimTable[activityN->resourceType][activityN->taskNumber] > currentResources[activityN->resourceType]
                    && resourceLockTable[activityN->resourceType][activityN->taskNumber] == 0){
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: \n\tIntial claim of %d EXCEEDS available unit(s) %d\n",
                               activityN->resourceAmount,
                               activityN->resourceType+1,
                               activityN->taskNumber+1,
                               resourceClaimTable[activityN->resourceType][activityN->taskNumber],
                               currentResources[activityN->resourceType]);
                        
                        currentState[activityN->resourceType][i] = DENIED;
                        // Increase the amount of time this task has been waiting
                        taskWaitingTable[activityN->taskNumber] += 1;
                        break;
                    }
                    
                    // If resource of this type is available and sufficient for request
                    if(initialResources[activityN->resourceType] >= activityN->resourceAmount
                       && currentResources[activityN->resourceType] >= activityN->resourceAmount) {
                        
                        currentResources[activityN->resourceType] = currentResources[activityN->resourceType] - activityN->resourceAmount;

                        nextResources[activityN->resourceType] -= activityN->resourceAmount;
                        if(nextResources[activityN->resourceType] < 0){
                            nextResources[activityN->resourceType] = 0;
                        }
                        
                        currentState[activityN->resourceType][i] = GRANTED;
                        printf("\nGRANTED %d unit(s) of resource %d to task %d.\n\tRemaining: %d\n\tAvailable next cycle: %d\t\n    ",
                               activityN->resourceAmount,
                               activityN->resourceType+1,
                               activityN->taskNumber+1,
                               currentResources[activityN->resourceType],
                               nextResources[activityN->resourceType]);
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
                // Set termination time
                if(taskTimeTable[i] == 0){
                    taskTimeTable[i] = cycle/numberOfTasks;
                }
                termCount++;
                break;
                
            default:
                break;
        }
        
        // On success move the pointer forward to the next activity in the linked list
        if (activityN->type != TERMINATE
        && currentState[activityN->resourceType][i] != WAITING
        && currentState[activityN->resourceType][i] != DENIED) {
            currentActivity[i] = currentActivity[i]->next;
        }
        
    }
    
	printf("\nBanker's DONE\n\n");
    
    printOutput(taskTimeTable, taskWaitingTable, numberOfTasks);
    
}