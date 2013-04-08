//
//  debugging.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/7/13.
//
//

#include <stdio.h>
#include "activity.h"


// Pass in a task table and its size to print the contents
void printQueues(activity** taskTable, int numberOfTasks){
    
    for (int i=0; i < numberOfTasks; i++) {
        printf("\nTask %d:\n", i+1);
        activity *tempActivity = taskTable[i];
        while (tempActivity != NULL) {
            printf("\t%2d %2d %10s %2d %2d\n",
                   tempActivity->taskNumber+1,
                   tempActivity->delay,
                   getActivityType(tempActivity->type),
                   tempActivity->resourceType,
                   tempActivity->resourceAmount);
            
            tempActivity = tempActivity->next;
        }
    }    
    
}