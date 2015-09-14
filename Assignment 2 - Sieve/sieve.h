/*
 * sieve.h
 *
 *  Created on: Feb 9, 2015
 *      Author: Andrew Gates and Brandon Watt
 */

#ifndef SIEVE_H_
#define SIEVE_H_
#include "queue.h"

struct prime_sieve {
	struct linked_queue *numbers;
	struct linked_queue *prime;
	int max;
};
typedef struct prime_sieve sieve;


void compute_to(sieve *s, int max);
void report_results(sieve *s);
int get_count(sieve *s);
int get_max(sieve *s);

#endif /* SIEVE_H_ */
