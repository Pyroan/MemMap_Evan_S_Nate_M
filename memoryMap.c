#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
/*****************************************
 *         Linux Memory Page Map         *
 * Evan Schoenberger & Nathaniel Manning *
 *****************************************/

// PID of process we're mapping.
int pid = -1;
// Reference to /proc/<<pid>>
DIR  *procDir;
// Files to be found in /proc/<<pid>>
FILE *pagemap;
FILE *maps;

/**
 * [2] Attempts to find corresponding directory in /proc.
 * closes program if unable to locate.
 */
void findPid()
{
	char *dirName;
	sprintf(dirName, "/proc/%d", pid); 
	procDir = opendir (dirName);
	if (procDir == NULL)
	{
		fprintf(stderr, "Error - /proc/%d not found\n", pid);
		exit(0);
	}
}

/**
 * [1] Gets initial PID input and makes sure it's the proper input type.
 */
void acceptInitialInput(char *input)
{
   // Set pid
	pid = atoi(input);
	
	// If input was not a valid integer, return an error and exit.
	if (pid == 0) // FIXME potential error if actually looking for process 0.
	{
		fprintf(stderr, "Error - Invalid PID: %s\n", input);
		exit(0);
	}
}

/**
 * main
 */
int main(int argc, char **argv)
{
	// [1]
	acceptInitialInput(argv[1]);
	// [2]
	findPid();
}
