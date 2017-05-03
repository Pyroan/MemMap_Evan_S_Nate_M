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
/*
typedef struct components{
	uint64 page_frame_number[55];
	uint64 swap_type_if_swapped[5];
	uint64 swap_offset_if_swapped[49];
	uint64 pte[1];
	uint64 page_exclusively_mapped[1];
	uint64 zero[3];
	uint64 file_page_or_shared_anon[1];
	uint64 swapped[1];
	uint64 present[1];
}; */

typedef struct map_entry {
long addr_start;
long addr_end;
char *protection;
long offset;
char *stuff;
long inode;
char *program;
struct map_entry *next;
} map_entry_t;

map_entry_t *head;
/*
 * [4] tokenizes the map, 
 */
void parseMaps()
{
   char str[128];
	fgets(str, 128, maps);
	while(fgets(str, 128, maps) != NULL)
   {
		// Create a new entry
		map_entry_t *entry = malloc(sizeof(map_entry_t));
		char *tok = strtok(str, " -");
		// Add start and end addresses		
		entry->addr_start = strtol(tok, NULL, 16);
      tok = strtok(NULL, " ");
		entry->addr_end = strtol(tok, NULL, 16);
//		printf("%lx, %lx\n", entry->addr_start, entry->addr_end);
		// Add permissions
		tok = strtok(NULL, " ");
		entry->protection = malloc(sizeof(tok));
		strcpy(entry->protection, tok);
		// Add offset
		tok = strtok(NULL, " ");
		entry->offset = strtol(tok, NULL, 16);
		// Add "stuff" TODO Please update this var name
		tok = strtok(NULL, " ");
		entry->stuff = malloc(sizeof(tok));
		strcpy(entry->stuff,tok);
		// Add inode
		tok = strtok(NULL, " ");
		entry->inode = strtol(tok, NULL, 16);
		// Add program
		do
		{
			tok = strtok(NULL, " ");
		} while (tok == NULL);
		entry->program = malloc(sizeof(tok));
		strcpy(entry->program, tok);
		
		// Add new entry to linked list
		map_entry_t *current = head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = entry;
   }
}

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
	head = malloc(sizeof(map_entry_t));
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
	// [4]
   parseMaps();
   
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
