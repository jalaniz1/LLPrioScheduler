#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>


struct node 
{
	int value;
	struct node *next;

};

struct node *root = NULL;
struct node *cur_proc = NULL;




/*
 * Function to add a process to the scheduler
 * @Param tid - the ID for the process/thread to be added to the 
 *      scheduler queue
 * @return true/false response for if the addition was successful
 */


int addProcess(int tid){
	
	if (root == NULL) // If null
	{
		//printf("Adding... %d\n", tid);
		root = (struct node*) malloc(sizeof(struct node));
		root->value = tid; // initialize
		root->next = NULL;
		return true; 
	}
	else
	{	struct node *cur = root;
		while(cur->next)
		{
			cur = cur->next;
			//printf("Loop\n");
		}// Must be null if we exit loop
		//printf("Adding... %d\n", tid);
		cur->next = (struct node*) malloc(sizeof(struct node));
		cur->next->value = tid; // Cur->next is initialized
		cur->next->next = NULL;
		return true;
	}
	return 0;
}

void printAll(){
	struct node *cur = root;

	while (cur)
	{
		printf("Ptr->value = %d\n", cur->value);
		cur = cur->next;
	}
}
/*
 * Function to remove a process from the scheduler queue
 * @Param tid - the ID for the process/thread to be removed from the
 *      scheduler queue
 * @Return true/false response for if the removal was successful
 */
int removeProcess(int tid){
struct node *cur = root;
	
	while(cur)
	{
		if(cur->value == tid)
		{
			struct node *tmp = cur;
			cur = cur->next; // Replace
			root = cur; // assuming we are always removing from the front
			free(tmp);
			//printf("Removing... %d\n", tid);
			return 1;
		}
		else
			cur = cur->next;
	}
	return 0;
}
/*
 * Function to get the next process from the scheduler
 * @Return returns the thread id of the next process that should be 
 *      executed, returns -1 if there are no processes
 */
int nextProcess(){
	if (root and cur_proc == NULL)
	{
		cur_proc = root; // First run
		return cur_proc->value;
	}
	else
	{
		cur_proc = cur_proc->next;
		if (root and cur_proc == NULL)
			cur_proc = root; // Restart
		if (cur_proc != NULL) // If not null
			return cur_proc->value;
	}


	return -1;
}
