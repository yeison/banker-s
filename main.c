#define MAIN_FILE
#include "main.h"

int verbose = 0;

int main (int argc, const char * argv[]) {
	if (argc < 2 || argc > 3) {
		//Explain proper usage of the program at the command line.
		printf("\tUsage: banker <name of input file> [--verbose] \n\n");
		exit(0);
	}
    
    char vstring[] = "--verbose";
    if( argv[2] != NULL && strcasecmp(argv[2], vstring)==0 ){
        verbose = 1;
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
    
    runOptimistic();
    
    stdin = fopen(argv[1], "r");
	runBankers();

}