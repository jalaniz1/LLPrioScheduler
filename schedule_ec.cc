#include "schedule_ec.h"
#include <stdio.h>
#include <stdlib.h>
struct node
{
	int value;
	int prio;
	struct node *next;
}*head;

/*
 * Function to add a process to the scheduler
 * @Param tid - the ID for the process/thread to be added to the 
 *      scheduler queue
 * @Param priority - the priority of the given process
 * @return true/false response for if the addition was successful
 */
int addProcessEC(int tid, int priority){
	
	if (head == NULL) // First case
	{
		
		head = (struct node*)malloc(sizeof(struct node));
		head->value = tid;
		head->prio = priority;
		return true;
	}
	else if (head->prio > priority) // Second case, priority is lower
	{
			
		struct node *ptr = head; // Store it
		head = (struct node *) malloc(sizeof(struct node));
		head->value = tid;
		head->prio = priority;
		head->next = ptr;	
		return true;
	}
	else
	{
		struct node *ptr = head;
		while(ptr->next and ptr->next->prio < priority)
		{
			ptr = ptr->next; // Move forward one
		}
		if (ptr->next == NULL) // If null then insert here
		{
		
			ptr->next = (struct node *)malloc(sizeof(struct node));
			ptr->next->value = tid;
			ptr->next->prio = priority;
		}
		else // Priority must be lower (higher)
		{
			
			struct node *tmp = ptr->next; // Store it
			ptr->next = (struct node *)malloc(sizeof(struct node));
			ptr->next->value = tid;
			ptr->next->prio = priority;
			ptr->next->next = tmp; // Restore it
		}
		return true;
	}

    return 0;
}
/*
 * Function to remove a process from the scheduler queue
 * @Param tid - the ID for the process/thread to be removed from the
 *      scheduler queue
 * @Return true/false response for if the removal was successful
 */
int removeProcessEC(int tid){
	struct node *ptr = head;
	while (ptr)
	{
		if (ptr->value == tid) // We have our target
		{
			struct node *tmp = ptr; // Save it for deletion
			ptr = ptr->next; // Replace it
			free(ptr);
			return true;
		}

		ptr = ptr->next;
	}
    return 0;
}
/*
 * Function to get the next process from the scheduler
 * @Return returns the thread id of the next process that should be 
 *      executed, returns -1 if there are no processes
 */
int nextProcessEC(){
	
    return -1;
}

void printAll()
{
	struct node *ptr = head; // Start at the head

	while(ptr) // While exists
	{
		printf("Pointer val %d and prio %d\n", ptr->value, ptr->prio);
		ptr = ptr->next;
	}
}
