#include <stdio.h>
#include "main.h"

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
	
	/************************************************************************/
	/*Begin Implementation*/
	
	int numberOfTasks;
	int numberOfResourceTypes;
	
	scanf("%d", &numberOfTasks);
	scanf("%d", &numberOfResourceTypes);
	
	printf("\n Number of Tasks: %d", numberOfTasks);
	printf("\n Number of Resource Types: %d", numberOfResourceTypes);
}
