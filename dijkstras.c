//  dijkstras.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/17/13.

#include "dijkstras.h"

// If my currently available resources are less than my claim, return false
bool allResourcesAreAvailable(int taskNumber, int numberOfResourceTypes, int *currentResources, int **resourceClaimTable,
                              int **resourceLockTable){
    
    for (int resourceType=0; resourceType < numberOfResourceTypes; resourceType++) {
        if (resourceClaimTable[resourceType][taskNumber] > currentResources[resourceType] + resourceLockTable[resourceType][taskNumber] ) {
            return false;
        }
    }
    
    return true;
}

activity* getNextActivity(activity** activityArray, queue* requestQueue){
    return activityArray[pop_front(requestQueue)];
}

void mergeQueues(queue** requestQueue, queue** nextRequestQueue, queue** helperRequestStack){

    if ((*requestQueue)->head != NULL) {
        printf("SOMETHING WENT WRONG");
        exit(EXIT_FAILURE);
    }

    while ((*helperRequestStack)->head != NULL) {
        int stackTop = pop_back(*helperRequestStack);
        push_front(stackTop, *nextRequestQueue);
    }
    
    *requestQueue = *nextRequestQueue;
    *nextRequestQueue = malloc(sizeof(queue));
}

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
    
    /* Request-queues keep track of pending-request order. */
    queue *requestQueue = malloc(sizeof(queue));
    for (int j=0; j < numberOfTasks; j++) {
        // Initialize with regular order: 0, 1, 2, 3, ...
        push(j, requestQueue);
    }
    queue *nextRequestQueue = malloc(sizeof(queue));
    queue *helperRequestStack = malloc(sizeof(queue));
    nextRequestQueue->head = NULL, nextRequestQueue->tail = NULL;
    helperRequestStack->head = NULL, helperRequestStack->tail = NULL;

    
    /*****************************************************************************/
    /* The meaty resource manager implementation */
    
    // Perform some initialization tasks
    activity **activityArray = malloc((numberOfTasks+1)*sizeof(activity *));
    for (int i=0; i < numberOfTasks; i++) {
        // Copy task-queue head pointers
        activityArray[i] = taskTable[i];
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
            mergeQueues(&requestQueue, &nextRequestQueue, &helperRequestStack);
            
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
                resolveDeadlock(resourceLockTable, numberOfTasks, numberOfResourceTypes, currentResources, activityArray, minRequest, taskTimeTable);
            }
            
            termCount = 0;
            waitingCount = 0;
            i = 0;
            printf("\nCycle: %d\n", (cycle/numberOfTasks));
            printf("**************\n");
        }
        
        // If there are no more activities for this task, continue to next task
        if(activityArray[i] == NULL){
            // Break if we have NULLs equal to numberOfTasks
            if(numberOfTasks == ++termCount){
                break;
            }
            continue;
        }
        
        activity *activityNode = getNextActivity(activityArray, requestQueue);
        
        // If the activity has a delay, skip it until the delay is exhausted
        if(activityNode->type != TERMINATE && activityNode->type != INITIATE && activityNode->delay > 0){
            activityNode->delay--;
            printf("\nTask %d skipping delayed activity: %s\n\tRemaining delay: %d\n",
                   activityNode->taskNumber+1,
                   getActivityType(activityNode->type),
                   activityNode->delay);
            
            currentState[activityNode->resourceType][activityNode->taskNumber] = DELAYED;
            push(activityNode->taskNumber, nextRequestQueue);

            continue;
        }
        
        switch (activityNode->type) {
            case INITIATE:
                // If claim exceeds total resource, then abort the task
                if(activityNode->resourceAmount > defaultResources[activityNode->resourceType]){
                    
                    printf("\nCould NOT claim %d unit(s) of resource %d for task %d: \n\tClaim EXCEEDS total available resources: %d\n",
                           activityNode->resourceAmount,
                           activityNode->resourceType+1,
                           activityNode->taskNumber+1,
                           defaultResources[activityNode->resourceType]);
                    abortTask(activityNode->taskNumber, resourceLockTable, numberOfResourceTypes, currentResources, activityArray);
                    currentState[activityNode->resourceType][activityNode->taskNumber] = TERMINATE;
                    taskTimeTable[activityNode->taskNumber] = -1;
                    push(activityNode->taskNumber, nextRequestQueue);
                    break;
                }
                // If state is 0 (not initiated) then initiate
                if (!previousState[activityNode->resourceType][i]) {
                    currentState[activityNode->resourceType][activityNode->taskNumber] = INITIATE;
                    // Retrieve and store the claim amount
                    resourceClaimTable[activityNode->resourceType][activityNode->taskNumber] = activityNode->resourceAmount;
                }
                push(activityNode->taskNumber, nextRequestQueue);
                break;
                
            case REQUEST:
                if(previousState[activityNode->resourceType][activityNode->taskNumber] >= INITIATE){
                    
                    // If request exceeds initial claims, then abort the task
                    if(activityNode->resourceAmount + resourceLockTable[activityNode->resourceType][activityNode->taskNumber] > resourceClaimTable[activityNode->resourceType][activityNode->taskNumber]){
                        
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: \n\tTotal resources requested %d EXCEEDS initial claim of %d\n",
                               activityNode->resourceAmount,
                               activityNode->resourceType+1,
                               activityNode->taskNumber+1,
                               activityNode->resourceAmount + resourceLockTable[activityNode->resourceType][activityNode->taskNumber],
                               resourceClaimTable[activityNode->resourceType][activityNode->taskNumber]);
                        abortTask(activityNode->taskNumber, resourceLockTable, numberOfResourceTypes, nextResources, activityArray);
                        currentState[activityNode->resourceType][activityNode->taskNumber] = TERMINATE;
                        taskTimeTable[activityNode->taskNumber] = -1; // Set time to aborted
                        push(activityNode->taskNumber, nextRequestQueue);
                        break;
                    }                                        
                    
                    // If initial claim exceeds available resources and resources have not already been allocated, deny request
                    if( (resourceClaimTable[activityNode->resourceType][activityNode->taskNumber] > currentResources[activityNode->resourceType]
                       && resourceLockTable[activityNode->resourceType][activityNode->taskNumber] == 0 )){
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: \n\tIntial claim of %d EXCEEDS available unit(s) %d\n",
                               activityNode->resourceAmount,
                               activityNode->resourceType+1,
                               activityNode->taskNumber+1,
                               resourceClaimTable[activityNode->resourceType][activityNode->taskNumber],
                               currentResources[activityNode->resourceType]);
                        
                        currentState[activityNode->resourceType][activityNode->taskNumber] = DENIED;
                        // Increase the amount of time this task has been waiting
                        taskWaitingTable[activityNode->taskNumber] += 1;
                        push(activityNode->taskNumber, helperRequestStack);
                        break;
                    }
                    
                    // If ALL resources are not currently available, deny request
                    if(!allResourcesAreAvailable(activityNode->taskNumber, numberOfResourceTypes, currentResources, resourceClaimTable, resourceLockTable)){
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: \n\tALL resources are not available \n",
                               activityNode->resourceAmount,
                               activityNode->resourceType+1,
                               activityNode->taskNumber+1);
                        
                        currentState[activityNode->resourceType][activityNode->taskNumber] = DENIED;
                        // Increase the amount of time this task has been waiting
                        taskWaitingTable[activityNode->taskNumber] += 1;
                        push(activityNode->taskNumber, helperRequestStack);
                        break;
                    }
                    
                    // If resource of this type is available and sufficient for request
                    if(initialResources[activityNode->resourceType] >= activityNode->resourceAmount
                       && currentResources[activityNode->resourceType] >= activityNode->resourceAmount) {
                        
                        currentResources[activityNode->resourceType] =
                            currentResources[activityNode->resourceType] - activityNode->resourceAmount;
                        
                        nextResources[activityNode->resourceType] -= activityNode->resourceAmount;
                        if(nextResources[activityNode->resourceType] < 0){
                            nextResources[activityNode->resourceType] = 0;
                        }
                        
                        currentState[activityNode->resourceType][activityNode->taskNumber] = GRANTED;
                        printf("\nGRANTED %d unit(s) of resource %d to task %d.\n\tRemaining: %d\n\tAvailable next cycle: %d\t\n    ",
                               activityNode->resourceAmount,
                               activityNode->resourceType+1,
                               activityNode->taskNumber+1,
                               currentResources[activityNode->resourceType],
                               nextResources[activityNode->resourceType]);
                        resourceLockTable[activityNode->resourceType][activityNode->taskNumber] += activityNode->resourceAmount;
                    } else {
                        printf("\nCould NOT grant %d unit(s) of resource %d to task %d: only %d available\n",
                               activityNode->resourceAmount,
                               activityNode->resourceType+1,
                               activityNode->taskNumber+1,
                               currentResources[activityNode->resourceType]);
                        currentState[activityNode->resourceType][activityNode->taskNumber] = WAITING;
                        waitingCount++;
                        taskWaitingTable[activityNode->taskNumber] += 1;
                        minRequest[activityNode->resourceType] = min(activityNode->resourceAmount, minRequest[activityNode->resourceType]);
                        push(activityNode->taskNumber, helperRequestStack);
                        break;
                    }
                } else {
                    perror("\nCannot perform REQUEST, previous state was not INITIATE.\n");
                    exit(2);
                }
                push(activityNode->taskNumber, nextRequestQueue);
                break;
                
            case RELEASE:
                if (previousState[activityNode->resourceType][activityNode->taskNumber] > INITIATE) {
                    nextResources[activityNode->resourceType] = nextResources[activityNode->resourceType] + activityNode->resourceAmount;
                    if(nextResources[activityNode->resourceType] > defaultResources[activityNode->resourceType]){
                        nextResources[activityNode->resourceType] = defaultResources[activityNode->resourceType];
                    }
                    currentState[activityNode->resourceType][activityNode->taskNumber] = RELEASE;
                    printf("\nTask %d RELEASED %d unit(s) of resource %d.\n\tUnits available next cycle: %d\n",
                           activityNode->taskNumber+1,
                           activityNode->resourceAmount,
                           activityNode->resourceType+1,
                           nextResources[activityNode->resourceType]);
                    resourceLockTable[activityNode->resourceType][activityNode->taskNumber] -= activityNode->resourceAmount;

                } else {
                    perror("\nCannot perform RELEASE, previous state was not REQUEST or RELEASE.\n");
                    exit(3);
                }
                
                push(activityNode->taskNumber, nextRequestQueue);
                break;
                
            case TERMINATE:
                // Set termination time
                if(taskTimeTable[activityNode->taskNumber] == 0){
                    taskTimeTable[activityNode->taskNumber] = cycle/numberOfTasks;
                }
                termCount++;
                push(activityNode->taskNumber, nextRequestQueue);
                break;
                
            default:
                break;
        }
        
        // On success move the pointer forward to the next activity in the linked list
        if (activityNode->type != TERMINATE
        && currentState[activityNode->resourceType][activityNode->taskNumber] != WAITING
        && currentState[activityNode->resourceType][activityNode->taskNumber] != DENIED) {            
            activityArray[activityNode->taskNumber] = activityArray[activityNode->taskNumber]->next;
        }
        
    }
    
	printf("\nBanker's DONE\n\n");
    
    printOutput(taskTimeTable, taskWaitingTable, numberOfTasks);
    
}