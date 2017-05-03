#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
/*****************************************
 *         Linux Memory Page Map         *
 * Evan Schoenberger & Nathaniel Manning *
 *****************************************/

// PID of process we're mapping.
int pid = 0;
// Reference to /proc/<<pid>>
char procDir[256];
// Files to be found in /proc/<<pid>>
FILE *pagemap;
FILE *maps;

/**
 * [5] Converts an address to its corresponding
 * page number.
 */
long findPageNo(long addr) 
{
	int pageSize = getpagesize();
	return addr/pageSize;
}

/**
 * [3] Finds pagemap and map references.
 * closes program with error if unable to locate.
 */
void findPidFiles()
{
	// Open pagemap
	pagemap = fopen("pagemap", "r");
	// Check for errors with pagemap
	if (pagemap == NULL)
	{
		if (errno = ENOENT)
			fprintf(stderr, "Error - pagemap not found\n");
		else
			fprintf(stderr, "Error - unable to open pagemap\n");
	}
	// open maps
	maps = fopen("maps", "r");
	// Check for errors with maps
	if (maps == NULL)
	{
		if (errno = ENOENT)
			fprintf(stderr, "Error - maps not found\n");
		else
			fprintf(stderr, "Error - unable to open maps\n");
	}
}

/**
 * [2] Attempts to find corresponding directory in /proc.
 * closes program with error if unable to locate.
 */
void findProcDir()
{
	// Set name of dir we're looking for
	sprintf(procDir, "/proc/%d", pid);
	// Attempt to open the dir.
	// Check for errors
	if (chdir(procDir) == -1)
	{
		fprintf(stderr, "Error - %s not found\n", procDir);
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
	if (pid == 0)
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
	// Take input, exit with error if no input provided.
	if (argc >=2)
	{
		acceptInitialInput(argv[1]);
	}
	else
	{
		fprintf(stderr, "Error - No input provided\n");
		exit(0);
	}
	// [2]
	// Check if the /proc/<<pid>> directory exists
	findProcDir();
	// [3] open the pagemap and maps files in /proc/<<directory>>
   findPidFiles();
   
   // For each entry we found in maps:
   map_entry_t currentEntry = head;
   while (currentEntry != NULL)
   {
   	// [5]
   	long startPage = findPageNo(currentEntry->addr_start);
   	long endPage = findPageNo(currentEntry->addr_end);
   	// Go through every page number in range of startPage to endPage,
   	// find its corresponding entry in the pagemap file.
   	for (int i = startPage, i <= endPage; i++)
   	{
   	}
   	currentEntry = currentEntry->next;
   }   
}
