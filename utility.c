//
//  utility.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/17/13.
//
//
#include "utility.h"

void abortTask(int taskNumber, int **resourceLockTable, int numberOfResourceTypes, int *nextResources, activity **currentActivity){
    printf("\nAborting task %d\n", taskNumber+1);
    
    for (int j = 0; j < numberOfResourceTypes; j++) {
        
        currentActivity[taskNumber]->type = TERMINATE;
                
        int resourceAmount = resourceLockTable[j][taskNumber];
        
        nextResources[j] += resourceAmount;
        printf("\tTask %d forced to release %d unit(s) of resource %d: %d units available next cycle\n",
               taskNumber+1,
               resourceAmount,
               j+1,
               nextResources[j]);
        
        resourceLockTable[j][taskNumber] = 0;
                
    }
}

bool resolveDeadlock(int **resourceLockTable, int numberOfTasks, int numberOfResourceTypes, int *currentResources, activity **currentActivity, int *minRequest, int *taskTimeTable){
    
    printf("\nResolving Deadlock\n");
    bool success = false;
    
    for (int i = 0; i < numberOfTasks; i++) {
        for (int j = 0; j < numberOfResourceTypes; j++) {
            if(resourceLockTable[j][i]){
                
                printf("\nAborting deadlocked task %d\n", i+1);
                currentActivity[i]->type = TERMINATE;
                taskTimeTable[i] = -1;
                
                while (j < numberOfResourceTypes) {
                    int resourceAmount = resourceLockTable[j][i];
                    
                    currentResources[j] += resourceAmount;
                    printf("\tTask %d forced to release %d unit(s) of resource %d: %d units available next cycle\n",
                           i+1,
                           resourceAmount,
                           j+1,
                           currentResources[j]);
                    
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

void printOutput(int *taskTimeTable, int *taskWaitingTable, int numberOfTasks){
    
    int totalTime = 0;
    int totalTimeWaiting = 0;

    printf("Output:\n");
    printf("Task    %6s %13s  %11s \n", "Time", "Time waiting", "% Waiting");
    for (int i=0; i < numberOfTasks; i++) {
        int time = taskTimeTable[i];
        
        if(time == -1){
            printf("Task %d: \taborted \n", i+1);
            continue;
        }
        
        int tWaiting = taskWaitingTable[i];
        
        totalTime += time;
        totalTimeWaiting += tWaiting;
        
        double percentWaiting = (1.0*tWaiting)/time;
        
        printf("Task %d: %6d %13d %11.2f%% \n", i+1, time, tWaiting, percentWaiting);
    }
    
    printf("Total: %7d %13d %11.2f%% \n\n\n", totalTime, totalTimeWaiting, (1.0*totalTimeWaiting)/totalTime);
    
}


// verbose printf function to use the verbose flag and call the standard printf if verbose=1
int	 verbose_printf(const char * __restrict string, ...){
    int result = 0;
    
    if(verbose){
        va_list args;
        va_start(args, string);
        result = printf(string, args);
        va_end(args);
    }
    
    return result;
        
}
