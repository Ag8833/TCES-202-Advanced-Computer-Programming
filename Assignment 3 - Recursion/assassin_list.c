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
		list_add_helper(list, curr, new_node);
	}

	strcpy(new_node->name, name);
}

void list_add_helper(alist list, struct assassin_node *curr, struct assassin_node *new_node)
{
	if(strcmp(list->kill_ring->name, curr->next->name) == 0)
	{
		// Append the new node to the end of the list.
		new_node->next = list->kill_ring;
		curr->next = new_node;
		return;
	}
	else
	{
		curr = curr->next;
		list_add_helper(list, curr, new_node);
		return;
	}
}

/**
 Prints the kill ring
 */
void print_kill_ring(alist list)
{
	struct assassin_node *check = list->kill_ring;
	struct assassin_node *temp = list->kill_ring;
	printf("\nCurrent kill ring:\n");
	print_kill_ring_helper(list, check, temp);
}

void print_kill_ring_helper(alist list, struct assassin_node *check, struct assassin_node *temp)
{
	printf("\t%s is stalking %s\n", list->kill_ring->name, list->kill_ring->next->name);
	temp=temp->next;

	if(temp==check)
	{
		return;
	}

	else
	{
		list->kill_ring=list->kill_ring->next;
		print_kill_ring_helper(list, check, temp);
	}
}

/**
 * Prints the grave yard
 */
void print_grave_yard(alist list)
{
	struct assassin_node *temp = list->grave_yard;
	printf("Current graveyard:\n");
	print_grave_yard_helper(list, temp);
}

void print_grave_yard_helper(alist list, struct assassin_node *temp)
{
	if(temp==NULL)
	{
		return;
	}

	else
	{
		printf("\t%s killed by %s\n", temp->name, temp->killer_name);
		temp=temp->next;
		print_grave_yard_helper(list, temp);
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

	if ( curr == NULL)
	{
		contains = 0;
	}
	else if (strncmp(curr->name, name, strlen(curr->name)) == 0)
	{
		contains = 1;
	}
	else
	{
		contains = grave_contains_helper(list, name , curr->next);
	}

	return contains;
}

int grave_contains_helper(alist list, char *name, struct assassin_node *curr)
{
	if (curr == NULL)
	{
		return 0;
	}
	else if (strncmp(curr->name, name, strlen(curr->name)) == 0)
	{
		return 1;
	}
	else
	{
		return grave_contains_helper(list, name, curr->next);
	}
}

/**
 * Returns 1 if kill ring contains a person
 * with the given name, 0 otherwise.
 */
int kill_ring_contains(alist list, char *name)
{
	int contains = 0;
	struct assassin_node *curr = list->kill_ring;


	if (strncmp(curr->name, name, strlen(curr->name)) == 0)
	{
		contains = 1;
	}
	else
	{
		contains = kill_contains_helper(list, name , curr->next);
	}

	return contains;
}

int kill_contains_helper(alist list, char *name,  struct assassin_node *curr)
{
	if (strcmp(list->kill_ring->name, curr->name) == 0)
	{
		return 0;
	}
	else if (strncmp(curr->name, name, strlen(curr->name)) == 0)
	{
		return 1;
	}
	else
	{
		return kill_contains_helper(list, name, curr->next);
	}

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
	if (list != NULL)
	{
		// Deallocating the grave_yard.

		//print_grave_yard(list);
		struct assassin_node *temp = list->grave_yard;

		while (temp != NULL)
		{
			if( temp->killer_name != NULL)
			{
				free(temp->killer_name);
				temp->killer_name = NULL;
			}

			free(temp->name);
			temp->name = NULL;

			struct assassin_node *curr = temp;
			temp = curr;
			temp = temp->next;


			free(curr);
			curr = NULL;
		}

		free(list->kill_ring->name);
		free(list->kill_ring);
		free(list);
	}

}
