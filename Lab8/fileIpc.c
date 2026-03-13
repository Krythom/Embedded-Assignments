#include <stdio.h>
#include <unistd.h>

void bgndFunc(FILE*);
void fgndFunc(FILE*);

/*
    File: fileIpc.c
    Author: Allison Marmura
    Date: 2026/03/12
    Description: This file contains a program that is forked into two separate threads.
	One thread detects keyboard inputs and creates a file associated with different inputs.
	The other thread waits to asee that a file has been created and then prints out a 
	confirmation and removes the file. The program can be quit using the input '5'.
*/

int main()
{
	int pid = fork();
	FILE* childPointer;
	FILE* parentPointer;
	
	if (pid == -1)
	{
		printf("Failed to fork.\n");
		return -1;
	}
	else if (pid == 0)
	{
		// Child
		bgndFunc(childPointer);
	}
	else
	{
		// Parent
		fgndFunc(parentPointer);
	}
}

/* Description: This function checks for the existence of various files
 * and either prints out a confirmation or quits the program when it sees one.
 * @param: A file pointer used to check for the existence of files, FILE*
 * @return: none
*/
void bgndFunc(FILE* id)
{
	int loop = 1;
	while (loop == 1) 
	{
		// DELAY ONCE PER LOOP //
		// SO PRINTS CAN BE SEEN //
		usleep(500000);
		id = fopen("1", "r");
		if (id != NULL) 
		{
			printf("BACKGROUND : Operation 1 complete\n");
			fclose(id);
			remove("1");
		}
		id = fopen("2", "r");
		if (id != NULL) 
		{
			printf("BACKGROUND : Operation 2 complete\n");
			fclose(id);
			remove("2");
		}
		id = fopen("3", "r");
		if (id != NULL) 
		{
			printf("BACKGROUND : Operation 3 complete\n");
			fclose(id);
			remove("3");
		}
		id = fopen("4", "r");
		if (id != NULL) 
		{
			printf("BACKGROUND : Operation 4 complete\n");
			fclose(id);
			remove("4");
		}
		id = fopen("5", "r");
		if (id != NULL) 
		{
			printf("Quitting Program\n");
			fclose(id);
			remove("5");
			loop = 0;
		}
	}
}

/* Description: This function checks for input from the user and creates
 * a file associated with the given input, or quits.
 * @param: A file pointer used to create the files, FILE*
 * @return: none
*/
void fgndFunc(FILE* id)
{
	int loop = 1;
	int operation;
	while (loop == 1) 
	{
		scanf("%d", &operation);
		if (operation == 1) 
		{
			id = fopen("1", "w");
			if (id == NULL) 
			{
				printf("ERROR : File failed to open"); 
			}
		fclose(id);
		} 
		else if (operation == 2) 
		{
			id = fopen("2", "w");
			if (id == NULL) 
			{
				printf("ERROR : File failed to open");
			}
			fclose(id);
		} 
		else if (operation == 3) 
		{
			id = fopen("3", "w");
			if (id == NULL) 
			{
				printf("ERROR : File failed to open");
			}
			fclose(id);
		} 
		else if (operation == 4) 
		{
			id = fopen("4", "w");
			if (id == NULL) 
			{
				printf("ERROR : File failed to open");
			}
			fclose(id);
		} 
		else if (operation == 5)
		{
			id = fopen("5", "w");
			if (id == NULL) 
			{
				printf("ERROR : File failed to open");
			}
			fclose(id);
			loop = 0;
		}
		else 
		{
			printf("ERROR : Illegal input");
		}
	}
}
