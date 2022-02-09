# simple_job_scheduler

## Author

Name: Duo Li

BlazerId: duo1998

Email: duo1998@uab.edu

- To compile: $ `gcc -Wall job_scheduler.c queue.c`
- To execute: > `./a.out 4`

## Sample output

```
$./a.out 2
You entered: 2 jobs
Enter command > help
Options:

        submit <program arguments> - Create a new process to execute the program specified with any arguments and print a jobid to standard output.
        showjobs - List all process that are either currently waiting or running
        submithistory - List all the processes that were executed by your job scheduler
        displayqueue - display the contents of the queue data structure for jobs list
        quit - quit the shell program
Enter command > showjobs
-----------------------------------------------------------
Job ID		 Command 		 status
-----------------------------------------------------------


Enter command > submit ./hw1 1000
job1 added to the queue
Enter command > submit ./hw1 1200
job2 added to the queue
Enter command > submit ./hw1 1400
job3 added to the queue
Enter command > showjobs
-----------------------------------------------------------
Job ID		 Command 		 status
-----------------------------------------------------------

1            ./hw1 1000              RUNNING
2            ./hw1 1200              RUNNING
3            ./hw1 1400              WAITING

Enter command > showjobs
-----------------------------------------------------------
Job ID		 Command 		 status
-----------------------------------------------------------

1            ./hw1 1400              RUNNING

Enter command > submithistory
submithistory command is required only for CS 532 students.
Enter command > quit
Terminate program
```
