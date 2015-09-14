/*
 * assassin_list.c
 *
 *  Created on: Dec 11, 2014
 *      Author: mmuppa
 * Edited by Andrew Gates and Brandon Watt
 */
#include "assassin_list.h"

void reaper(alist, anode, anode);
/**
 Creates a new node with the given name and 
 adds to the kill ring which is a circular kill ring.
 */
void list_add(alist list, char *name)
{
	struct assassin_node *curr = list->kill_ring;

	// Create node to be added.
	struct assassin_node *new_node = malloc(sizeof(struct assassin_node));
	new_node->name = calloc(strlen(name), sizeof(char));
	new_node->killer_name = calloc(50, sizeof(char));


	// If this is the first node added to the kill ring.
	if (list->kill_ring == NULL)
	{
		list->kill_ring = new_node;
		new_node->next = list->kill_ring;
	}
	else
	{
		// If there is only one node in the list.
		if(list->kill_ring->next == list->kill_ring)
		{
			new_node->next = list->kill_ring;
			curr->next = new_node;
		}
		else
		{
			// Traverse off of the first node.
			// prev = curr;
			curr = curr->next;

			// Traverse to the end of the list.
			while(curr->next != list->kill_ring)
			{
				curr = curr->next;
			}

			// Append the new node to the end of the list.
			new_node->next = list->kill_ring;
			curr->next = new_node;
		}

	}

	strcpy(new_node->name, name);
}

/**
 Prints the kill ring
 */
void print_kill_ring(alist list)
{
	struct assassin_node *curr = list->kill_ring;

	printf("\nCurrent kill ring:\n");

	// Traverse to the end of the list.
	do
	{
		printf("\t%s is stalking %s\n", curr->name, curr->next->name);
		curr = curr->next;
	} while(curr != list->kill_ring);
}

/**
 * Prints the grave yard
 */
void print_grave_yard(alist list)
{
	struct assassin_node *curr = list->grave_yard;

	printf("Current graveyard:\n");

	while (curr != NULL)
	{
		printf("\t%s killed by %s\n", curr->name, curr->killer_name);
		curr = curr->next;
	}
}

/**
 * Returns 1 if graveyard contains a person
 * with the given name, 0 otherwise.
 */
int grave_yard_contains(alist list, char *name)
{
	int contains = 0;
	struct assassin_node *curr = list->grave_yard;

	while (curr != NULL)
	{
		if (strncmp(curr->name, name, strlen(curr->name)) == 0)
		{
			contains = 1;
		}

		curr = curr->next;
	}
	return contains;
}

/**
 * Returns 1 if kill ring contains a person
 * with the given name, 0 otherwise.
 */
int kill_ring_contains(alist list, char *name)
{
	int contains = 0;
	struct assassin_node *curr = list->kill_ring;

	do
	{
		if(strncmp(curr->name, name, strlen(curr->name)) == 0)
		{
			contains = 1;
		}
		curr = curr->next;

	} while(curr != list->kill_ring);

	return contains;
}
/**
 * Returns 1 if game is over, only one person left in the
 * kill ring. 0 otherwise.
 */
int game_over(alist list)
{
	int done = 0;

	if (list->kill_ring->next == list->kill_ring)
	{
		done = 1;
	}

	return done;
}

/**
 * If game is over, returns the name of the winner.
 * Otherwise, NULL.
 */
char *winner(alist list)
{
	return list->kill_ring->name;
}

/**
 * Moves the person from the kill ring into the
 * graveyard and keeps the kill ring intact.
 */
void kill_victim(alist list, char *name)
{
	struct assassin_node *curr = list->kill_ring;
	struct assassin_node *prev = list->kill_ring;

	// Traverse list until you find.
	while(strncmp(curr->name, name, strlen(curr->name)) != 0)
	{
		prev = curr;
		curr = curr->next;
	}

	// If the node to be removed is the head node.
	if (curr == list->kill_ring)
	{
		prev = list->kill_ring->next;
		// Move prev to the "end" of the list.
		while (prev->next != list->kill_ring)
		{
			prev = prev->next;
		}

		prev->next = curr->next;
		list->kill_ring = curr->next;

		reaper(list, curr, prev);

	}
	else
	{
		prev->next = curr->next;
		reaper(list, curr, prev);
	}
}

void reaper(alist list, anode victim, anode murderer)
{
	// Assign the killers name

	strcpy(victim->killer_name, murderer->name);

	// Place victim at the head of the graveyard.
	if (list->grave_yard == NULL)
	{
		victim->next = NULL;
		list->grave_yard = victim;
	}
	else
	{
		victim->next = list->grave_yard;
		list->grave_yard = victim;
	}

}

/**
 * Release the memory allocated for each node, name, killer_name.
 */
void deallocate(alist list)
{
	struct assassin_node *curr = list->kill_ring;

	// Deallocating the kill ring.
	while (curr != NULL)
	{
		free(curr);
		curr = NULL;
	}


	// Deallocating the grave_yard.
	struct assassin_node *temp = list->grave_yard;

	while (temp != NULL)
	{
		struct assassin_node *curr2 = temp;
		temp = temp->next;
		free(curr2);
	}

	free(list);
}
