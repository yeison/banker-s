//
//  debugging.c
//  bankers
//
//  Created by Yeison Rodriguez on 4/7/13.
//
//

#include <stdio.h>
#include "activity.h"


void printQueues(activity** taskTable, int numberOfTasks){
    
    for (int i=0; i < numberOfTasks; i++) {
        printf("\nTask %d:\n", i+1);
        activity *tempActivity = taskTable[i];
        while (tempActivity != NULL) {
            printf("\tThe type of this activity is: %s \n", getActivityType(tempActivity->type));
            tempActivity = tempActivity->next;
        }
    }    
    
}