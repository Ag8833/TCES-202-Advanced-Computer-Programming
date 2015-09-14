/*
 * compute.c
 *
 *  Created on: Mar 3, 2015
 *      Author: ag8833
 *      Edited by: Andrew Gates and Brandon Watt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "global.h"
#include "screen_utils.h"
#include "file.h"
#include "compute.h"

// Calls the appropriate calculation function.
int compute_main_menu()
{
	int selection = 0;
	int error = NO_ERROR;

	// Calling the select calculation function.
	while (selection != EXIT_COMP)
	{
		clear_screen();
		selection = compute_menu();
		switch (selection)
		{
		case COMP_STATS:
		{
			error = compute_update();
			if (error)
				printf("File error: Check that file is created\n");
			press_enter();
			break;
		}
		case COMP_MEAN:
		{
			error = compute_mean();
			if (error)
				printf("File error: Check that file is created\n");
			press_enter();
			break;
		}
		case COMP_MEDIAN:
		{
			error = compute_median();
			if (error)
				printf("File error: Check that file exists\n");
			press_enter();
			break;
		}
		case COMP_VARIANCE:
		{
			error = compute_variance();
			if (error)
				printf("File error: Check that file exists\n");
			press_enter();
			break;
		}

		case COMP_STD_DEV:
		{
			error = compute_std_dev();
			if (error)
				printf("File error: Check that file exists\n");
			press_enter();
			break;
		}

		}
	}
	clear_screen();
	return error;
}

// Gets the user input to call the appropriate function.
int compute_menu(void)
{
	char sel_s[3];
	int sel;

	print_compute_menu();
	gets(sel_s);
	sel = atoi(sel_s);
	if (sel < COMP_STATS || sel > EXIT_COMP)
	{
		while (1)
		{
			clear_screen();
			printf("Incorrect selection. Please enter one of these choices.\n");
			print_compute_menu();
			gets(sel_s);
			sel = atoi(sel_s);
			if (!(sel < COMP_STATS || sel > EXIT_COMP))
				break;
		}
	}
	clear_screen();
	return sel;
}

// Prints the compute menu.
int print_compute_menu(void)
{
	printf("StatPac File menu options.\n");
	printf(
			"File Menu: 1) Update All 2) Compute Mean 3) Compute Median 4) Compute Variance \n5) Compute Standard Devitation 6) Exit\n");
	printf("Please choose an option: ");
	fflush(stdout);
	return 0;
}

// Gets the data from the file.
void getData(double *data, FileHeaderPtr fh_p, char* filename, int* error)
{
	char headerStr[1000];
	char *pos;
	FILE *fid;

	*error = check_file_name(filename);
	if (!*error)
	{
		fid = fopen(filename, "r");
		if (!fid) {
			// error in opening file
			printf(
					"Error in opening file %s: Check to make sure filename is correct and file exists.\n",
					filename);
			return; // a return here means we don't need an else clause
		}

		// Gets header record.
		get_file_header(fh_p, fid);

		// Prints header record in a formatted version.
		char buffer[256];
		printf("%s\n", to_string_file_header(fh_p, buffer));

		// Getting file data
		int i;
		for (i = 1; i <= fh_p->num_records; i++)
		{
			// get all but last record
			headerStr[0] = 0;
			fgets(headerStr, 99, fid);		// use for data records
			pos = strchr(headerStr, '\n');
			if (pos)
				*pos = 0;
			data[i - 1] = atof(headerStr);
			printf("%d: %f\n", i, data[i - 1]);
			fflush(stdout);
		}

		fclose(fid);
	}
}

// Edits the file updating it's data.
void edit_file(double *data, FileHeaderPtr fh_p, char* filename)
{
	// Create a temporary file and put new header record in it.
	FILE *fid_temp;
	fid_temp = fopen("temp.dat", "w");
	fprintf(fid_temp, "%s,%s,%s,%s,%ld,%s,%f,%f,%f,%f\n", fh_p->filename,
			fh_p->creation_date, fh_p->modification_date, fh_p->owner,
			fh_p->num_records, "entry", fh_p->mean, fh_p->median,
			fh_p->variance, fh_p->std_dev);

	// Put data into temporary file.
	int i;
	for (i = 1; i <= fh_p->num_records; i++)
	{
		if (i == fh_p->num_records)
			fprintf(fid_temp, "%f", data[i - 1]);
		else
			fprintf(fid_temp, "%f\n", data[i - 1]);
	}

	// Rename temporary file to original file, overwriting it.
	fclose(fid_temp);
	remove(filename);
	rename("temp.dat", filename);
}

// Computes all the statistics.
int compute_update(void)
{
	char filename[91];
	int error = 0;
	double sum = 0;
	double temp_var = 0;
	double temp_value = 0;
	double temp_median = 0;

	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
	double data[100];

	printf("Enter the file name for updating data: ");
	gets(filename);
	printf("\n");

	getData(data, fh_p, filename, &error);

	double copy[fh_p->num_records];
	int i;
	for (i = 1; i <= fh_p->num_records; i++)
	{
		copy[i - 1] = data[i - 1];
		sum += data[i - 1];
	}

	// Median
	median_sort(copy, fh_p->num_records);

	if (fh_p->num_records % 2 == 0)
	{
		temp_median += copy[fh_p->num_records / 2];
		temp_median += copy[(fh_p->num_records - 1) / 2];
		fh_p->median = (temp_median / 2);
	}
	else
	{
		fh_p->median = copy[fh_p->num_records / 2];
	}

	// Mean
	fh_p->mean = sum / fh_p->num_records;

	// Variance
	for (i = 1; i <= fh_p->num_records; i++)
	{
		temp_value = fh_p->mean - data[i - 1];
		temp_var += (temp_value * temp_value);
	}

	fh_p->variance = temp_var / fh_p->num_records;

	// Standard Deviation.
	fh_p->std_dev = sqrt(fh_p->variance);

	edit_file(data, fh_p, filename);

	printf("\nThe mean is: %f", fh_p->mean);
	printf("\nThe median is: %f", fh_p->median);
	printf("\nThe variance is: %f", fh_p->variance);
	printf("\nThe standard deviation is: %f\n", fh_p->std_dev);
	printf("Updating complete. Closing %s\n", filename);
	free(fh_p);

	return error;
}

// Gets file data computes the mean and updates the file.
int compute_mean(void)
{
	int error = 0;
	char filename[91];
	int sum = 0;

	printf("Enter the file name for computing mean data: ");
	gets(filename);
	printf("\n");

	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
	double data[100];

	getData(data, fh_p, filename, &error);

	int i;
	for (i = 1; i <= fh_p->num_records; i++)
	{
		sum += data[i - 1];
	}

	fh_p->mean = sum / fh_p->num_records;

	edit_file(data, fh_p, filename);

	printf("\nThe mean is: %f\n", fh_p->mean);
	printf("Computing Mean complete. Closing %s\n", filename);
	free(fh_p);

	return error;
}

// Gets file data computes the median and updates the file.
int compute_median(void)
{
	int error = 0;
	char filename[91];
	double temp_median = 0;

	printf("Enter the file name for computing median data: ");
	gets(filename);
	printf("\n");

	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
	double data[100];

	getData(data, fh_p, filename, &error);

	double copy[fh_p->num_records];

	int i;
	for (i = 1; i <= fh_p->num_records; i++)
	{
		copy[i - 1] = data[i - 1];
	}

	// Sorts the data into ascending order.
	median_sort(copy, fh_p->num_records);

	if (fh_p->num_records % 2 == 0)
	{
		temp_median += copy[fh_p->num_records / 2];
		temp_median += copy[(fh_p->num_records - 1) / 2];
		fh_p->median = (temp_median / 2);
	}
	else
	{
		fh_p->median = copy[fh_p->num_records / 2];
	}

	edit_file(data, fh_p, filename);

	printf("\nThe median is: %f\n", fh_p->median);
	printf("Computing Median complete. Closing %s\n", filename);
	free(fh_p);

	return error;
}

// Returns 0 if the values are equal, negative if a is less than b and positive if a is greater than b.
int compare(const void * a, const void * b)
{
	return (*(double*) a - *(double*) b);
}

// Call qsort using the compare function to sort the data.
void median_sort(double* orig, long size)
{
	qsort(orig, size, sizeof(double), compare);
}

// Gets file data computes the variance and updates the file.
int compute_variance(void)
{
	int error = 0;
	char filename[91];
	double temp_var = 0;
	double temp_value = 0;

	printf("Enter the file name for calculating variance data: ");
	gets(filename);
	printf("\n");

	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
	double data[100];

	getData(data, fh_p, filename, &error);

	int i;
	for (i = 1; i <= fh_p->num_records; i++)
	{
		temp_value = fh_p->mean - data[i - 1];
		temp_var += (temp_value * temp_value);
	}

	fh_p->variance = temp_var / fh_p->num_records;

	edit_file(data, fh_p, filename);

	printf("\nThe variance is: %f\n", fh_p->variance);
	printf("Computing Variance complete. Closing %s\n", filename);
	free(fh_p);

	return error;
}

// Gets file data computes the standard deviation and updates the file.
int compute_std_dev(void)
{
	int error = 0;
	char filename[91];

	printf("Enter the file name for standard deviation data: ");
	gets(filename);
	printf("\n");

	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
	double data[100];

	getData(data, fh_p, filename, &error);

	fh_p->std_dev = sqrt(fh_p->variance);

	edit_file(data, fh_p, filename);

	printf("\nThe standard deviation is: %f\n", fh_p->std_dev);
	printf("Computing standard deviation complete. Closing %s\n", filename);
	free(fh_p);

	return error;
}
