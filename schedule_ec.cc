#include "schedule_ec.h"
#include <stdio.h>
#include <stdlib.h>
struct node
{
	int value;
	int prio;
	int arr_index;  // Required for proper removal and deletion
	struct node *next;
}*head, **n_array; // **n_array is a pointer to struct node pointers

size_t group_size = 0;  // Keeps track of the max group sizes
int cur_proc = 0; // Keeps track of the current proc
 



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
		head->arr_index = -1; // Initial Array index for removal
		return true;
	}
	else if (head->prio > priority) // Second case, priority is lower
	{
			
		struct node *ptr = head; // Store it
		head = (struct node *) malloc(sizeof(struct node));
		head->value = tid;
		head->prio = priority;
		head->next = ptr;
		head->arr_index = -1; // Initial Array index for removal	
		return true;
	}
	else
	{
		struct node *ptr = head;
		while(ptr->next and ptr->next->prio <= priority)
		{
			ptr = ptr->next; // Move forward one
		}
		if (ptr->next == NULL) // If null then insert here
		{
			ptr->next = (struct node *)malloc(sizeof(struct node));
			ptr->next->value = tid;
			ptr->next->prio = priority;
			ptr->next->arr_index = -1;// Initial Array index for removal
		}
		else // Priority must be lower (higher)
		{
			
			struct node *tmp = ptr->next; // Store it
			ptr->next = (struct node *)malloc(sizeof(struct node));
			ptr->next->value = tid;
			ptr->next->prio = priority;
			ptr->next->arr_index = -1; // Initial Array index for removal
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
	
	/**** Modified version of the removal checks to see there is a node array (n_array)
		index value associated with a pointer being removed.  If there is, then we
		must free the n_array[i] node and set it to NULL for our nextProcessEC()
		to work correctly.  Else, we aren't removing a node with an associated
		struct node **n_array[index]
	****/
	if (head->value == tid) // First node
	{
		
		struct node *tmp = head;
		head = head->next;
		if (tmp->arr_index != -1) // Delete the array spot as well
		{
			int i = tmp->arr_index;
			free(n_array[i]);
			n_array[i] = NULL;
		}
		else
			free(tmp); // Delete the old head

		tmp = NULL;
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
		else if (ptr->next->value == tid) // Wehave found our target
		{
			
			struct node *tmp = ptr->next; // Save it
			ptr->next = ptr->next->next; // Move it up one

			if (tmp->arr_index != -1) // Delete the array spot as well
			{
				int i = tmp->arr_index;
				free(n_array[i]);
				n_array[i] = NULL;
			}
			else
				free(tmp); // Delete
			tmp = NULL;
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
	/**** First IF statement.  Here, if the node array(n_array) and head pointer exist.
		Then we go through our n_array finding the next appropriate (and non-null)
		process id to return.  Should the n_array be empty upon a previous deletion,
		we will go to the second if statement to create *another* subgroup.
		Implying by the time we enter this first IF statement, a subgroup has already been created
		previously in the second IF statement  ****/

	if (n_array and head)
	{
		int i =0;
		bool empty = true; // Default empty
		bool cur_set = false; // Default false, not set
		if (cur_proc == group_size-1)
		{
			cur_proc = 0; // Reset before we hit the loop
			
			if(n_array[cur_proc]) // If it exists, go ahead and lock it
			{
				cur_set = true;
				empty = false; // Not empty from the get go
			}
		}
		for(;i < group_size and !cur_set;i++)
		{
			if (n_array[i] and cur_proc < i )
			{
				empty = false; // Not empty
				cur_proc = i; // Cur proc is now equal to index of the for loop
				break; // We no longer need to continue, break out for performance
			}
		}
		
		if (empty)// Our group is empty, it's been fully deleted. Reset for a new group
		{
			free(n_array); // Clean it and mark it for deletion
			n_array = NULL; // Set it to NULL, reset
			cur_proc = 0; // Reset current proc index
		}
		else			
			return n_array[cur_proc]->value; // If not empty then we have a cur_proc to return
	}

	/**** Second IF statement, it is crucial to the algorithm that we do not make this else if.  
		In case we require resetting the array, we can by going to this if statement 
		Here in this IF statement block is where we make a new 'subgroup' of processes to run 
		It is a First-Come First-Serve algorithm utilizing the priority queue to make subgroups ****/
	
	if (n_array == NULL and head) // (Re)Initialize n_array
	{
		struct node *ptr = head;
		group_size = 1; // Head's value accounted for
		struct node * last = head; // Initialize last
		head->arr_index = group_size-1; // Set array index of pointer for deletion and clean-up
		n_array = (struct node **)malloc(group_size * sizeof(struct node*));
		n_array[group_size-1] = last; // Last is set to head at this point
	
		while(ptr)
		{
			ptr = ptr->next; // Advance forward
			while(ptr and ptr->prio == last->prio) // While ptr exists and ptr priority is equal to last priority
				ptr = ptr->next; // Keep moving forward 
			if (ptr) // Not at end of list
			{
				group_size += 1; // Add to our subgroup
				last = ptr; // Our last ptr, in case we need it, is set to PTR
				n_array = (struct node **)realloc(n_array, group_size * sizeof(struct node*)); // Re-size the array
				n_array[group_size-1] = last; // Set it equal to last(ptr)
				last->arr_index = group_size-1; // Set array index of pointer for deletion and clean-up
			}
		}
		return n_array[cur_proc]->value;
	}
	    return -1;// We must have failed!
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
