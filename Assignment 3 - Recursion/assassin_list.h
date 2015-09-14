// Edited by Andrew Gates and Brandon Watt

#ifndef ASSASSIN_LIST_H_INCLUDED
#define ASSASSIN_LIST_H_INCLUDED

#include "assassin_node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct assassin_list
{
    struct assassin_node *kill_ring;
    struct assassin_node *grave_yard;
};

typedef struct assassin_list *alist;

void list_add(alist, char *);
void list_add_helper(alist list, struct assassin_node *curr, struct assassin_node *new_node);
void print_kill_ring(alist);
void print_kill_ring_helper(alist list, struct assassin_node *check, struct assassin_node *temp);
void print_grave_yard(alist);
void print_grave_yard_helper(alist list, struct assassin_node *temp);
int game_over(alist);
char *winner(alist);
int grave_yard_contains(alist, char *);
int grave_contains_helper(alist list, char *name, struct assassin_node *curr);
int kill_ring_contains(alist, char *);
int kill_contains_helper(alist list, char *name,  struct assassin_node *curr);
void kill_victim(alist, char *);
void reaper(alist list, anode victim, anode murderer);
void deallocate(alist);

#endif // ASSASSIN_LIST_H_INCLUDED

