/*
 *Name: Allegra Thompson
 *ID: athomp22
 *Date: March 16, 2019
 *Assignment: CIS3110 A3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for PCBs
typedef struct PCB
{
	int process;
	char state[10];
	int runTime;
	int readyTime;
	int blockTime;
	struct PCB * next;
} pcb;

//struct for Events
typedef struct Event
{
	int time;
	char event;
	int resource;
	int processID;
} events;

//struct for Queues
typedef struct Queue
{
	pcb * front;
	pcb * rear;
} queue;

//function prototypes
int parseInput(char in[200][5], int max);
pcb * newPCB(int p);
events * newEvent(int t, char e, int r, int p);
queue * createQueue();
void enQueue(queue * q, pcb * newPCB);
pcb * deQueue(queue * q);
int findPCB(pcb * array[20], int p, int numPCB);
int checkRun(pcb * array[20], int numPCB);
void calcTime(int index, pcb * p);
int checkExit(pcb * array[20], int numPCB);
void printTimes(pcb * array[20], int numPCB);
void printStates(pcb * array[20], int numPCB);

//global array of events
events * eventArray[50];

int main()
{
	//inputs from file
	int ch;
	char input[200][5];
	//counters
	int i, j, index, max, numEvents, numPCB;
	//array of PCBs
	pcb * array[20];
	//temporary PCB
	pcb * temp;

	//ready and 5 resources queues
	queue * ready = createQueue();
	queue * r1 = createQueue();
	queue * r2 = createQueue();
	queue * r3 = createQueue();
	queue * r4 = createQueue();
	queue * r5 = createQueue();

	i = 0;

	//read in file and save strings to input
	do {
		scanf("%s", input[i]);
		ch = fgetc(stdin);
		i++;
	} while (ch != EOF);
	max = i - 1;

	//converting input into events in eventArray
	numEvents = parseInput(input, max);

	//initializing idle state and initil time
	array[0] = newPCB(0);
	strcpy(array[0]->state, "run");
	array[0]->runTime = array[0]->runTime + eventArray[0]->time;
	numPCB = 1;


	//execute events
	for (i = 0; i < numEvents; i++)
	{
		//printf("Time: %d, event: %c, process: %d\n", eventArray[i]->time, eventArray[i]->event, eventArray[i]->processID);
		//setting index of array with same process as specified in event
		index = findPCB(array, eventArray[i]->processID, numPCB);

		//selecting for event
		switch (eventArray[i]->event)
		{
			//create new process
			case 'C':
				temp = newPCB(eventArray[i]->processID);

				//set state to run if no other PCBs running
				if (checkRun(array, numPCB) == 0)
				{
					strcpy(temp->state, "run");
				}
				//otherwise set state to ready
				else
				{
					strcpy(temp->state, "ready");
					temp->next = NULL;
					enQueue(ready, temp);
				}
				//add new PCB to array and increase PCB count
				array[numPCB] = temp;
				numPCB++;
				break;
			//process exits
			case 'E':
				//if process not running, dequeue process
				if (strcmp(array[index]->state, "run") != 0)
				{
					//select queue for dequeing
					switch(eventArray[i]->resource)
					{
						//dequeue r1
						case 1:
							temp = deQueue(r1);
							while (temp->process != array[index]->process)
							{
								temp->next = NULL;
								enQueue(r1, temp);
								temp = deQueue(r1);
							}
							break;
						//dequeue r2
						case 2:
							temp = deQueue(r2);
							while (temp->process != array[index]->process)
							{
								temp->next = NULL;
								enQueue(r2, temp);
								temp = deQueue(r2);
							}
							break;
						//dequeue r3
						case 3:
							temp = deQueue(r3);
							while (temp->process != array[index]->process)
							{
								temp->next = NULL;
								enQueue(r3, temp);
								temp = deQueue(r3);
							}
							break;
						//dequeue r4
						case 4:
							temp = deQueue(r4);
							while (temp->process != array[index]->process)
							{
								temp->next = NULL;
								enQueue(r4, temp);
								temp = deQueue(r4);
							}
							break;
						//dequeue r5
						case 5:
							temp = deQueue(r5);
							while (temp->process != array[index]->process)
							{
								temp->next = NULL;
								enQueue(r5, temp);
								temp = deQueue(r5);
							}
							break;
						//dequeue ready
						default:
							if (ready->front != NULL)
							{
								temp = deQueue(ready);
							}
							break;
					}
					//set exiting process state to exit
					strcpy(temp->state, "exit");
				}
				//otherwise process running
				else
				{
					//set exiting process state to exit
					strcpy(array[index]->state, "exit");

					//check if ready queue empty
					if (ready->front != NULL)
					{
						temp = deQueue(ready);
						//set process state to run
						if (strcmp(temp->state, "ready") == 0)
						{
							strcpy(temp->state, "run");
						}
					}
				}

				//check if all processes have exited and at end of event array
				if ((checkExit(array, numPCB) == 0) && (i >= numEvents - 1))
				{
					//print each process' running, ready and block times
					printTimes(array, numPCB);

					//free array of PCBs
					for (j = 0; j < numPCB; j++)
					{
						free(array[j]);
					}
					//free queues
					free(ready);
					free(r1);
					free(r2);
					free(r3);
					free(r4);
					free(r5);;
					//free event array
					for (j = 0; j < numEvents; j++)
					{
						free(eventArray[j]);
					}
					//exit program
					return 0;
				}

				break;
			//process requests resource
			case 'R':
				//if process was in ready state, dequeue ready queue
				if (strcmp(array[index]->state, "ready") == 0)
				{
					if (ready->front != NULL)
					{
						temp = deQueue(ready);

					}
				}
				//select for resource queue and enqueue process
				switch (eventArray[i]->resource)
				{
					//resource 1
					case 1:
						temp->next = NULL;
						enQueue(r1, array[index]);
						break;
					//resource 2
					case 2:
						temp->next = NULL;
						enQueue(r2, array[index]);
						break;
					//resource 3
					case 3:
						temp->next = NULL;
						enQueue(r3, array[index]);
						break;
					//resource 4
					case 4:
						temp->next = NULL;
						enQueue(r4, array[index]);
						break;
					//resource 5
					case 5:
						temp->next = NULL;
						enQueue(r5, array[index]);
						break;
				}

				//change state to block
				strcpy(array[index]->state, "block");

				//if no processes running, dequeue ready
				if (checkRun(array, numPCB) == 0)
				{
					//if ready not empty
					if (ready->front != NULL)
					{
						temp = deQueue(ready);
						//set process state to run
						if (strcmp(temp->state, "ready") == 0)
						{
							strcpy(temp->state, "run");
						}
					}
				}
				break;
			//interrupt from resource
			case 'I':
				//select for resource queue and dequeue
				switch (eventArray[i]->resource)
				{
					//resource 1
					case 1:
						temp = deQueue(r1);
						//loop until process IDs match
						while (temp->process != array[index]->process)
						{
							temp->next = NULL;
							enQueue(r1, temp);
							temp = deQueue(r1);
						}
						break;
					//resource 2
					case 2:
						temp = deQueue(r2);
						//loop until process IDs match
						while (temp->process != array[index]->process)
						{
							temp->next = NULL;
							enQueue(r2, temp);
							temp = deQueue(r2);
						}
						break;
					//resource 3
					case 3:
						temp = deQueue(r3);
						//loop until process IDs match
						while (temp->process != array[index]->process)
						{
							temp->next = NULL;
							enQueue(r3, temp);
							temp = deQueue(r3);
						}
						break;
					//resource 4
					case 4:
						temp = deQueue(r4);
						//loop until process IDs match
						while (temp->process != array[index]->process)
						{
							temp->next = NULL;
							enQueue(r4, temp);
							temp = deQueue(r4);
						}
						break;
					//resource 5
					case 5:
						temp = deQueue(r5);
						//loop until process IDs match
						while (temp->process != array[index]->process)
						{
							temp->next = NULL;
							enQueue(r5, temp);
							temp = deQueue(r5);
						}
						break;
				}

				//if no processes running, set state to run
				if (checkRun(array, numPCB) == 0)
				{
					strcpy(temp->state, "run");
				}
				//otherwise set state to ready and enqueue ready
				else
				{
					strcpy(temp->state, "ready");
					temp->next = NULL;
					enQueue(ready, temp);
				}
				break;
			//timer interrupt
			case 'T':
				//if ready queue not empty
				if (ready->front != NULL)
				{
					//retrieve currently running process and set to ready
					temp = array[checkRun(array, numPCB)];
					strcpy(temp->state, "ready");
					temp->next = NULL;
					enQueue(ready, temp);

					//dequeue ready and set process to run
					temp = deQueue(ready);
					strcpy(temp->state, "run");
					eventArray[i]->processID = temp->process;
					index = findPCB(array, eventArray[i]->processID, numPCB);
				}
				break;
			//default is ignore
			default:
				break;
		}

		//if no processes running, set idle process to run
		if (checkRun(array, numPCB) == 0)
		{
			strcpy(array[0]->state, "run");
		}
		//otherwise idle process is idle
		else
		{
			strcpy(array[0]->state, "idle");
		}

		//for all processes, calculate time
		for (j = 0; j < numPCB; j++)
		{
			calcTime(i, array[j]);
		}
	}

	return 0;
}


//parseInput converts input array of strings into array of events
//Inputs: char in[][]: array of strings, int max: size of input array
//Outputs: int: number of events
int parseInput(char in[200][5], int max)
{
	int i, j;
	int t, r, p;
	char e;

	i = 0;
	j = 0;

	//looping for whole array
	while (i < max)
	{
		//if create or exit event types, use this format
		if ((strcmp(in[i + 1], "C") == 0) || (strcmp(in[i + 1], "E") == 0))
		{
			t = atoi(in[i]);
			e = in[i + 1][0];
			r = 0;
			p = atoi(in[i + 2]);
			i = i + 3;
		}
		//if request or interrupt event types, use this format
		else if ((strcmp(in[i + 1], "R") == 0) || (strcmp(in[i + 1], "I") == 0))
		{
			t = atoi(in[i]);
			e = in[i + 1][0];
			r = atoi(in[i + 2]);
			p = atoi(in[i + 3]);
			i = i + 4;
		}
		//if timer interrupt event type, use this format
		else if (strcmp(in[i + 1], "T") == 0)
		{
			t = atoi(in[i]);
			e = in[i + 1][0];
			r = 0;
			p = -1;
			i = i + 2;
		}
		//add event to event array
		eventArray[j] = newEvent(t, e, r, p);
		j++;
	}
	
	return j;
}

//creates new PCB object
//Inputs: int p: process ID of new PCB
//Outputs: pcb *: pointer to newly created PCB
pcb * newPCB(int p)
{
	pcb * temp = (pcb *)malloc(sizeof(pcb));
	temp->process = p;
	strcpy(temp->state, "new");
	temp->runTime = 0;
	temp->readyTime = 0;
	temp->blockTime = 0;
	temp->next = NULL;
	return temp;
}

//creates new event object
//Inputs: int t: new event time, char e: new event type, int r: new event resource, int p: new event process
//Outputs: events *: pointer to newly created event
events * newEvent(int t, char e, int r, int p)
{
	events * temp = (events *)malloc(sizeof(events));
	temp->time = t;
	temp->event = e;
	temp->resource = r;
	temp->processID = p;
	return temp;
}

//creates new queue object
//Inputs: none
//Outputs: queue *: pointer to newly created queue
queue * createQueue()
{
	queue * q = (queue *)malloc(sizeof(queue));
	q->front = NULL;
	q->rear = NULL;
	return q;
}

//adds PCB to queue
//Inputs: queue * q: queue element will be added to, pcb * newPCB: pcb element that will be added to queue
//Outputs: none
void enQueue(queue * q, pcb * newPCB)
{
	//if queue is empty, then new node is front and rear
	if (q->rear == NULL)
	{
		q->front = newPCB;
		q->rear = newPCB;
		return;
	}

	//add new node at the end of queue and change rear
	q->rear->next = newPCB;
	q->rear = newPCB;
}

//removes a PCB from a queue
//Inputs: queue * q: queue element will be removed from
//Outputs: pcb *: element removed from queue
pcb * deQueue(queue * q)
{
	//if queue is empty return NULL
	if (q->front == NULL)
	{
		return NULL;
	}

	//store previous front and move front one node ahead
	pcb * temp = q->front;
	q->front = q->front->next;

	//if front becomes NULL, then change rear to NULL
	if (q->front == NULL)
	{
		q->rear = NULL;
	}
	return temp;
}

//locates PCB in array with specified process ID
//Inputs: pcb * array[]: array of PCBs, int p: process ID to be found, int numPCB: size of PCB array
//Output: int: index of PCB array where process ID is found or 1 if process ID not found
int findPCB(pcb * array[10], int p, int numPCB)
{
	int i;

	//loop for PCB array
	for (i = 0; i < numPCB; i++)
	{
		//if process ID found, return index
		if (array[i]->process == p)
		{
			return i;
		}
	}
	return 0;
}

//checks for running process
//Inputs: pcb * array[]: array of PCBs, int numPCB: size of PCB array
//Outputs: int: index of running PCB or 0 if no PCBs running
int checkRun(pcb * array[10], int numPCB)
{
	int i;

	//loop for PCB array
	for (i = 1; i < numPCB; i++)
	{
		//if process running, return index
		if (strcmp(array[i]->state, "run") == 0)
		{
			return i;
		}
	}
	return 0;
}

//calculates run/ready/block time of PCB
//Inputs: int index: index of event in event array, pcb * p: calculate time increase for this PCB
//Outputs: none
void calcTime(int index, pcb * p)
{
	int newTime;

	//calculate how long event at index runs for
	newTime = eventArray[index + 1]->time - eventArray[index]->time;

	//if PCB state is run, increase run time
	if (strcmp(p->state, "run") == 0)
	{
		p->runTime = p->runTime + newTime;
	}
	//if PCB state is ready, increase ready time
	else if (strcmp(p->state, "ready") == 0)
	{
		p->readyTime = p->readyTime + newTime;
	}
	//if PCB state is block, increase block time
	else if (strcmp(p->state, "block") == 0)
	{
		p->blockTime = p->blockTime + newTime;
	}
}

//checks if all PCBs in array are in exit state
//Inputs: pcb * array[]: array of PCBs, int numPCB: size of PCB array
//Outputs: int: index of PCB in array not in exit state, or 0 if all PCBs in exit state
int checkExit(pcb * array[20], int numPCB)
{
	int i;

	//loop for PCB array
	for (i = 1; i < numPCB; i++)
	{
		//if PCB not in exit state, return index
		if (strcmp(array[i]->state, "exit") != 0)
		{
			return i;
		}
	}
	return 0;
}

//prints the run/ready/block times for all PCBs in array
//Inputs: pcb * array[]: array of PCBs, int numPCB: size of PCB array
//Outputs: none
void printTimes(pcb * array[20], int numPCB)
{
	int i;

	//print system idle process run time
	printf("%d %d\n", array[0]->process, array[0]->runTime);

	//loop for PCB array excluding system idle process 0
	for (i = 1; i < numPCB; i++)
	{
		printf("%d %d %d %d\n", array[i]->process, array[i]->runTime, array[i]->readyTime, array[i]->blockTime);
	}
}

//prints the states for all PCBs in array
//Inputs: pcb * array[]: array of PCBs, int numPCB: size of PCB array
//Outputs: none
void printStates(pcb * array[20], int numPCB)
{
	int i;

	//loop for PCB array
	for (i = 0; i < numPCB; i++)
	{
		printf("process %d state %s\n", array[i]->process, array[i]->state);
	}
}
