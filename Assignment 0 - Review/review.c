/**
 * Andrew Gates
 * TCES202
 * Winter 2015
 * This program reads numbers from a file, taking the first number as the file length. Then it returns a reference to the
 * array of numbers which is then sent to the function process numbers which takes these numbers and using a struct fills
 * the stats of the numbers, which is then returned and then printed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BUFFER_LEN 10

struct number_stats
{
	double pos_sum;
	int pos_cnt;
	double pos_ave;
	int pos_abv_ave_cnt;
	double neg_sum;
	int neg_cnt;
	double neg_ave;
	int neg_abv_ave_cnt;
};
typedef struct number_stats Stat;

double *read_numbers_from_file(const char *, int *);
Stat *process_numbers(const double *, int);
void print_numbers(const Stat *);

/**
 * main function!
 */
int main(int argc, const char * argv[])
{
	double *numbers;
	Stat *stats;
	int length = 0;
	numbers = read_numbers_from_file("real1.txt", &length);
	if (numbers != NULL)
	{
		stats = process_numbers(numbers, length);
		if (stats != NULL)
		{
			print_numbers(stats);
			free(stats);
		}
		free(numbers);
	}
	return 0;
}

/**
 * Prints the statistics from the Positive and negative values.
 */
void print_numbers(const Stat *s)
{
	printf("\nPositive sum=%.1f, count=%d, average=%.1f, count above average=%d\n",s->pos_sum,s->pos_cnt,s->pos_ave,s->pos_abv_ave_cnt);
	printf("\nNegative sum=%.1f, count=%d, average=%.1f, count above average=%d\n",s->neg_sum,s->neg_cnt,s->neg_ave,s->neg_abv_ave_cnt);
}

/**
 * Allocate the Stat variable, iterate through the array to tally up
 * the counts, sums, averages, and above average count and return the
 * reference to Stat. Returns NULL if unable to allocate Stat variable.
 */
Stat *process_numbers(const double *numbers, int size)
{
	//Allocating the structure named stats based on the file length
	Stat*stats=calloc(size,sizeof(Stat));
	int i;
	for (i=0;i<size;i++)
	{
		printf("%.1f\n",numbers[i]);
	}
	for (i=0;i<size;i++)
	{
		if (numbers[i]>=0)
		{
			stats->pos_cnt++;
			stats->pos_sum+=numbers[i];
		}
		else
		{
			stats->neg_cnt++;
			stats->neg_sum+=numbers[i];
		}
	}
	stats->pos_ave=(stats->pos_sum)/(stats->pos_cnt);
	stats->neg_ave=(stats->neg_sum)/(stats->neg_cnt);
	for (i=0;i<size;i++)
	{
		if (numbers[i]>=0 && numbers[i]>stats->pos_ave)
		{
			stats->pos_abv_ave_cnt++;
		}
		else if (numbers[i]<0 && numbers[i]>stats->neg_ave)
		{
			stats->neg_abv_ave_cnt++;
		}
	}
	return stats;
}

/**
 * Reads the numbers from the file, allocates the array
 * and initializes the values. Returns the reference to the array
 * if able to allocate and populate the array otherwise a NULL.
 */
double *read_numbers_from_file(const char *file_name, int *length)
{
	file_name=fopen("real1.txt","r");
	int i=0;
	float temp_number=0;
	double *numbers;
	if (file_name==NULL)
	{
		printf("Error opening file\n");
	}
	else
	{
		//Getting the file length
		fscanf(file_name, "%d", length);
		//Allocating the array based on the file length
		numbers=calloc(*length,sizeof(double));
		for (i=0;i<*length;i++)
		{
			fscanf(file_name, "%f", &temp_number);
			numbers[i]=temp_number;
		}
		fclose(file_name);
		return numbers;
	}
	return 0;
}
