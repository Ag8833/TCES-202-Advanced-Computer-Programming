/*
 * sieve.c
 *
 *  Created on: Feb 9, 2015
 *      Author: Andrew Gates and Brandon Watt
 */

#include "sieve.h"
#include <math.h>

void compute_to(sieve *s, int max)
{
	s->max = max;
	s->numbers = malloc(sizeof(struct linked_queue));
	s->prime = malloc(sizeof(struct linked_queue));

	int i;
	for (i = 2; i <= max; i++)
	{
		enqueue(s->numbers, i);
	}

	int p;
	queue *temp = malloc(sizeof( struct linked_queue));

	do {
		//obtain the next prime p by removing the first value in the queue of numbers.
		p = dequeue(s->numbers);
		//put p into the queue of primes.
		enqueue(s->prime, p);


		int size = queue_size(s->numbers);
		int x;
		//go through the queue of numbers, eliminating numbers divisible by p.
		for (x = 0; x < size; x++)
		{
			int t = dequeue(s->numbers);

			if (t % p != 0)
			{
				enqueue(temp, t);
			}
		}
		// Put stored prime values into the numbers queue.
		s->numbers = temp;

	} while (p < sqrt(max));

	// all remaining values in numbers queue are prime, so transfer them to primes queue
	int size = queue_size(s->numbers);
	int z;
	for (z = 0; z < size; z++)
	{
		enqueue(s->prime, dequeue(s->numbers));
	}

	// Returns numbers queue to original state.
	for (i = 2; i <= max; i++)
	{
		enqueue(s->numbers, i);
	}

	temp = NULL;
	free(temp);
}


void report_results(sieve *s)
{
	queue *temp = malloc(sizeof( struct linked_queue));
	printf("Primes up to %d are as follows:\n", s->max);

	int size = get_count(s);
	int i;
	for (i = 1; i <= size; i++)
	{
		int t = dequeue(s->prime);
		enqueue(temp, t);

		// If t > 1000 only use one space.
		if (t / 1000 > 0)
		{
			printf(" %d", t);
		}
		// Two spaces for 100 < t < 1000
		else if (t / 100 > 0)
		{
			printf("  %d", t);
		}
		// Three spaces for 10 < t < 100
		else if (t / 10 > 0)
		{
			printf("   %d", t);
		}
		// Four spaces for t < 10
		else
		{
			printf("    %d", t);
		}

		// Start a new line if 12 numbers have been printed.
		if( i % 12 == 0)
		{
			printf("\n");
		}
	}

	printf("\n");
	s->prime = temp;
	temp = NULL;
	free(temp);
}

int get_count(sieve *s)
{
	return queue_size(s->prime);
}

int get_max(sieve *s)
{
	// Deallocate queues.
	s->prime = NULL;
	s->numbers = NULL;
	free(s->prime);
	free(s->numbers);
	return s->max;
}
