#include "schedule_ec.h"
#include <stdio.h>
#include <stdlib.h>
struct node
{
	int value;
	int prio;
	struct node *next;
}*head, *last;

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
	
	
	if (head->value == tid) // First node
	{
		
		struct node *tmp = head;
		head = head->next;
		free(tmp); // Delete the old head
		return true;
	}
	else
	{
		
		struct node *ptr = head;
		while (ptr->next and ptr->next->value != tid)
		{
			ptr = ptr->next;
		}
		if (ptr->next == NULL)
			return false; // Couldn't find it, end of list
		else if (ptr->next->value == tid) // We must have found our target
		{

			struct node *tmp = ptr->next; // Save it
			ptr->next = ptr->next->next; // Move it up one
			free(tmp);
			return true;
		}
	}
	
	return 0;
}
/*
 * Function to get the next process from the scheduler
 * @Return returns the thread id of the next process that should be 
 *      executed, returns -1 if there are no processes
 */
int nextProcessEC(){

	if (last == NULL) // Initialize for the first time
	{
		last = head;
		return last->value;
	}

	else
	{
		last = last->next; // Move it forward to the next one
		if (last == NULL) 
			last = head; // Reset
		if (last != NULL)
			return last->value;
	}

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
