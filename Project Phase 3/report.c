/*
 * report.c
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

#include "global.h"
#include "screen_utils.h"
#include "file.h"
#include "report.h"

// Calls report and captures user input for options selection.
int report_main_menu()
{
	int selection = 0;
	int error = NO_ERROR;

	// Exits when the user enters 2.
	while (selection != EXIT_REPORT)
	{
		clear_screen();
		selection = report_menu();
		switch (selection)
		{
		case REPORT_DISPLAY:
		{
			error = report_data();
			if (error)
				printf("File error: Check name\n");
			press_enter();
			break;
		}
		}
	}
	clear_screen();
	return error;
}

// Takes in user input and opens appropriate function.
int report_menu(void)
{
	char sel_s[3];
	int sel;

	print_report_menu();
	gets(sel_s);
	sel = atoi(sel_s);
	if (sel < REPORT_DISPLAY || sel > EXIT_REPORT)
	{
		while (1)
		{
			clear_screen();
			printf("Incorrect selection. Please enter one of these choices.\n");
			print_report_menu();
			gets(sel_s);
			sel = atoi(sel_s);
			if (!(sel < REPORT_DISPLAY || sel > EXIT_REPORT))
				break;
		}
	}
	clear_screen();
	return sel;
}

// The menu displayed.
int print_report_menu(void)
{
	printf("StatPac Report menu options.\n");
	printf("Report Menu: 1) Report Statistics 2) Exit\n");
	printf("Please choose an option: ");
	fflush(stdout);
	return 0;
}

int report_data(void)
{
	char filename[91];
	FILE *fid;

	printf("Enter the file name for reporting statistics: ");
	gets(filename);
	printf("\n");

	int error = check_file_name(filename);
	if (!error)
	{
		fid = fopen(filename, "r");
		if (!fid)
		{
			// error in opening file
			printf(
					"Error in opening file %s: Check to make sure filename is correct and file exists.\n",
					filename);
			return FILE_ERROR; // a return here means we don't need an else clause
		}

		// Getting the data from the file.
		FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));
		get_file_header(fh_p, fid);

		char buffer[256];
		printf("%s", to_string_file_header(fh_p, buffer));

		fclose(fid);

		printf("\nReporting Statistics for %s", filename);
		printf("\nThe mean is: %f", fh_p->mean);
		printf("\nThe median is: %f", fh_p->median);
		printf("\nThe variance is: %f", fh_p->variance);
		printf("\nThe standard deviation is: %f\n", fh_p->std_dev);
		printf("Reporting Statistics complete. Closing %s\n", filename);
		free(fh_p);
	}

	return error;
}
