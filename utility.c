//
//  utility.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/17/13.
//
//
#include "utility.h"


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