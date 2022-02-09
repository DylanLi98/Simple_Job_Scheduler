#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "queue.h"

// Author: Duo Li
// To compile: $ gcc -Wall job_scheduler.c queue.c
// To execute: > ./a.out 4

int createarray(char *line, char *aptr[])
{
	int count = 0;
	char *token, *temp;

	temp = (char *)malloc(sizeof(char) * (strlen(line) + 1));
	strcpy(temp, line);
	for (;; temp = NULL)
	{
		if ((token = strtok(temp, " ")) == NULL)
			break;
		aptr[count++] = token;
	}
	aptr[count] = (char *)NULL;
	return count;
}

typedef struct job
{
	int jobid; // job id
	char *command1;
	char *command2;
	char *status;
	char **aptr;
} Job;

#define MAXTOKENS 100
int size = 0;
Job *list;
int numOfjobs;

void handler(int sig)
{
	int i;
	size--;
	printf("job is done\n");
	for (i = 1; i <= size; i++)
	{
		list[i] = list[i + 1];
		list[i].jobid = list[i].jobid - 1;
	}
	if (size <= numOfjobs)
	{
		list[size].status = "RUNNING";
	}
}

int main(int argc, char **argv)
{
	char line[BUFSIZ], *aptr[MAXTOKENS], buffer[BUFSIZ];
	int count, n, inx, i;
	pid_t pid;
	FILE *jobout, *joberr;
	list = (Job *)malloc(sizeof(Job) * 100);

	if (argc < 2)
	{
		printf("Usage: Enter an integer\n");
		exit(-1);
	}

	numOfjobs = atoi(argv[1]);
	printf("You entered: %d jobs\n", numOfjobs);
	queue *job_list = queue_init(numOfjobs);

	struct sigaction act;
	act.sa_handler = handler;
	sigaction(SIGCHLD, &act, NULL);

	for (;;)
	{
		printf("Enter command > ");
		if (fgets(line, BUFSIZ, stdin) != NULL)
		{
			line[strlen(line) - 1] = '\0';
			count = createarray(line, aptr);
			if (count < 1)
				continue;
			if (strcmp(line, "quit") == 0)
			{
				printf("Terminate program\n");
				break;
			}
			else if (strcmp(line, "help") == 0)
			{
				printf("Options: \n\n");
				printf("\tsubmit <program arguments> - Create a new process to execute the program specified with any arguments and print a jobid to standard output.\n");
				printf("\tshowjobs - List all process that are either currently waiting or running\n");
				printf("\tsubmithistory - List all the processes that were executed by your job scheduler\n");
				printf("\tdisplayqueue - display the contents of the queue data structure for jobs list\n");
				printf("\tquit - quit the shell program\n");
			}
			else if (strcmp(aptr[0], "submit") == 0)
			{
				if (count < 2)
				{
					printf("submit requires an <program arguments>\n");
				}
				else
				{
					aptr[0] = aptr[1];
					aptr[1] = aptr[2];
					aptr[2] = NULL;
					size++;
					list[size].jobid = size;
					list[size].command1 = aptr[0];
					list[size].command2 = aptr[1];
					list[size].aptr = aptr;
					if (size <= numOfjobs && (n = queue_insert(job_list, job_list->count)) != -1)
					{ // queue is not null or full
						list[size].status = "RUNNING";
						pid = fork();
						if (pid == 0)
						{ // child process
							// replace standard output stream with the file jobid.out
							sprintf(buffer, "job%d", job_list->count);
							inx = strlen(buffer);
							strcpy(&buffer[inx], ".out");
							if ((jobout = freopen(buffer, "w", stdout)) == NULL)
							{
								printf("Error opening file for output\n");
								exit(-1);
							}
							// replace standard error stream with the file jobid.err
							strcpy(&buffer[inx], ".err");
							if ((joberr = freopen(buffer, "w", stderr)) == NULL)
							{
								printf("Error opening file for error\n");
								exit(-1);
							}
							execvp(aptr[0], aptr);
							perror("execv");
							exit(-1);
						}
						else if (pid > 0)
						{ // parent process
							queue_delete(job_list);
						}
					}
					else if (size > numOfjobs)
					{
						list[size].status = "WAITING";
					}
					printf("job%d added to the queue\n", size);
				}
			}
			else if (strcmp(line, "showjobs") == 0)
			{
				printf("-----------------------------------------------------------\n");
				printf("Job ID\t\t Command \t\t status\t\n");
				printf("-----------------------------------------------------------\n\n");
				for (i = 1; i <= size; i++)
				{
					printf("%d\t\t %s %s\t\t %s\n", list[i].jobid, list[i].command1, list[i].command2, list[i].status);
				}
			}
			else if (strcmp(line, "submithistory") == 0)
			{
				printf("submithistory command is required only for CS 532 students.\n");
			}
			else if (strcmp(line, "displayqueue") == 0)
			{
				queue_display(job_list);
			}
		}
	}
	return 0;
}