/*
 * Edited by Andrew Gates and Brandon Watt
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global.h"
#include "edit.h"
#include "file.h"
#include "screen_utils.h"

/* edit the contents of an open file */
int edit_main_menu(void)
{
	int selection = 0;
		int error = NO_ERROR;

		while (selection != EXIT_SEL)
		{
			clear_screen();
			selection = edit_menu();
			switch (selection)
			{
			case EDIT_ADD_SEL:
			{
				error = edit_add();
				if (error)
					printf("File error: Check that file is created\n");

				press_enter();
				break;
			}
			case EDIT_EDIT_SEL: {
				error = edit_edit();
				if (error)
					printf("File error: Check that file is created\n");
				press_enter();
				break;
			}
			case EDIT_DELETE_SEL: {
				error = edit_delete();
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

// Validation for the edit menu selection
int edit_menu(void) {
    char sel_s[3];
    int sel;
    
    print_edit_menu();
    gets(sel_s);
    sel = atoi(sel_s);
    if ((sel < EDIT_ADD_SEL || sel > EDIT_DELETE_SEL) && sel != EXIT_SEL) {
        while (1) {
            clear_screen();
            printf("Incorrect selection. Please enter one of these choices.\n");
            print_edit_menu();
            gets(sel_s);
            sel = atoi(sel_s);
            if (!((sel < EDIT_ADD_SEL || sel > EDIT_DELETE_SEL)
                  && sel != EXIT_SEL))
                break;
        }
    }
    return sel;
}

// Prints the different edit menu choices.
void print_edit_menu(void)
{
	printf("StatPac Edit menu options.\n");
	printf("Edit Menu: 1) Add new data 2) Edit existing data 3) Delete data 6) Exit\n");
	printf("Please choose an option: ");
	fflush(stdout);
}


/**********************************************************************************************/
/* allows users to add any number of new data items to the file.
 */
int edit_add(void)
{
    // Appends data directly to the end of the file.
	char filename[MAX_FILE_NAME_LEN];
	FILE *edit;
	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(struct file_header));

	clear_screen();
	printf("Enter the file name for adding data: ");
	fflush(stdout);
	gets(filename);

	FILE *get_data = fopen(filename, "r");

	// Check that file exists.
	if (get_data == NULL)
	{
		return FILE_ERROR;
	}

	// Prints normal file data.
	get_file_header(fh_p, get_data);
	char buff[256];
	printf("%s", to_string_file_header(fh_p, buff));
	printf("\nFile: %s open for adding data\n", filename);
	fflush(stdout);

	fclose(get_data);

	// Read in normal file data and ignore.
	get_data = fopen(filename, "r");
	fgets(buff, 255 ,get_data);

	// Read in any previously added data.
	double data[100];
	int i;
	for (i = 0; i < fh_p->num_records; i++)
	{
		fscanf(get_data,"%lf", &data[i]);
	}

	fclose(get_data);
	printf("Press enter to start adding data.\n");
	press_enter();
	fflush(stdout);

	// Set count in case of previously entered data.
	int count = fh_p->num_records;
	// An array to take in input.
	char addition[50];

	printf("Enter data ('q' to quit): ");
	fflush(stdout);
	gets(addition);


	while (strcmp(addition, "q") != 0)
	{
		count++;
		double newdata = atof(addition);
		data[count-1] = newdata;
		printf("Enter data ('q' to quit): ");
		fflush(stdout);
		gets(addition);
	}

	edit = fopen(filename, "r+");
	fh_p->num_records = count;

	// Gets new modification date.
	char date[26];
	time_t timer;
	struct tm *timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	strcpy(date, (char *) asctime(timeinfo));
	date[24] = 0;
	strcpy(fh_p->modification_date, date);

	// Overwrite file with new basic file data.
	fprintf(edit, "%s,%s,%s,%s,%ld,%s,%f,%f,%f,%f\n", fh_p->filename,
					fh_p->creation_date, fh_p->modification_date, fh_p->owner,
					fh_p->num_records, "entry", fh_p->mean, fh_p->median, fh_p->variance,
					fh_p->std_dev);

	// Add new data.
	for (i = 0; i < count; i++)
	{
		fprintf(edit, "%lf\n", data[i]);
	}

	printf("Adding data complete. Closing %s\n", filename);
	fclose(edit);
	free(fh_p);
	return NO_ERROR;
}

/** Edit data
 */

int edit_edit()
{
	char filename[MAX_FILE_NAME_LEN];
	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(struct file_header));

	clear_screen();
	printf("Enter the file name for editing data: ");
	fflush(stdout);
	gets(filename);

	FILE *get_data = fopen(filename, "r");

	// Check that file exists.
	if (get_data == NULL)
	{
		return FILE_ERROR;
	}

	// Prints normal file data.
	get_file_header(fh_p, get_data);
	char buff[400];
	printf("%s\n", to_string_file_header(fh_p, buff));

	// Takes in and prints out data from file.
	double data[fh_p->num_records];
	int i;
	for(i = 0; i < fh_p->num_records; i++)
	{
		fscanf(get_data, "%lf", &data[i]);
		printf("%d: %lf\n", i+1, data[i]);
	}

	int input = 1;
	printf("\nWhich record would you like to edit? ");
	while (input != 0)
	{
		printf("\nEnter the record number or 0 to quit : 1 - %ld: ", (fh_p->num_records));
		scanf("%d", &input);

		// If the input is 0 or not within the array bounds, repeat request.
		if (input != 0 && input > 0 && input < fh_p->num_records + 1)
		{
			printf("\nEnter new value for record %d data %lf: ", input, data[input-1]);
			scanf("%lf", &data[input-1]);
		}

	}

	fclose(get_data);

	get_data = fopen(filename, "r+");

	// Gets new modification date.
	char date[26];
	time_t timer;
	struct tm *timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	strcpy(date, (char *) asctime(timeinfo));
	date[24] = 0;
	strcpy(fh_p->modification_date, date);

	// Add updated standard file info.
	fprintf(get_data, "%s,%s,%s,%s,%ld,%s,%f,%f,%f,%f\n", fh_p->filename,
						fh_p->creation_date, fh_p->modification_date, fh_p->owner,
						fh_p->num_records, "entry", fh_p->mean, fh_p->median, fh_p->variance,
						fh_p->std_dev);

	// Add edited data.
	for (i = 0; i < fh_p->num_records; i++)
	{
		fprintf(get_data, "%lf\n", data[i]);
	}

	fclose(get_data);
	fflush(stdout);

	return NO_ERROR;
}

/**
 Delete entries from file
 */
int edit_delete()
{
	char filename[MAX_FILE_NAME_LEN];
	FileHeaderPtr fh_p = (FileHeaderPtr) malloc(sizeof(struct file_header));

	clear_screen();
	printf("Enter the file name for deleting data: ");
	fflush(stdout);
	gets(filename);

	FILE *get_data = fopen(filename, "r");

	// Check that file exists.
	if (get_data == NULL)
	{
		return FILE_ERROR;
	}

	// Prints normal file data.
	get_file_header(fh_p, get_data);
	char buff[400];
	printf("%s\n", to_string_file_header(fh_p, buff));

	// Takes in and prints out data from file.
	double data[fh_p->num_records];
	int i;
	for(i = 0; i < fh_p->num_records; i++)
	{
		fscanf(get_data, "%lf", &data[i]);
		printf("%d: %lf\n", i+1, data[i]);
	}

	int input = 1;
	int count = fh_p->num_records;
	// Storing the original amount of data so the full array can be checked later.
	int temp = count;

	while (input != 0)
	{
		printf("\nWhich record would you like to delete? Enter the record number or 0 to quit : 1 - %ld:", fh_p->num_records);
		scanf("%d", &input);

		// If the input is 0 or not within the array bounds, repeat request.
		if (input != 0 && input > 0 && input < fh_p->num_records + 1)
		{
			data[input-1]='~'; //Assigning the value to be deleted to some character to check later.
			count--;
		}
	}

	fclose(get_data);
	// Reopen file to overwrite data.
	get_data = fopen(filename, "w");

	// Gets new modification date.
	char date[26];
	time_t timer;
	struct tm *timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	strcpy(date, (char *) asctime(timeinfo));
	date[24] = 0;
	strcpy(fh_p->modification_date, date);

	// Update record count.
	fh_p->num_records = count;
	fprintf(get_data, "%s,%s,%s,%s,%ld,%s,%f,%f,%f,%f\n", fh_p->filename,
						fh_p->creation_date, fh_p->modification_date, fh_p->owner,
						fh_p->num_records, "entry", fh_p->mean, fh_p->median, fh_p->variance,
						fh_p->std_dev);

	fclose(get_data);
	get_data = fopen(filename, "a");

	// Go through the data array and ignore data that has been "deleted".
	for (i = 0; i < temp; i++)
	{
		if(data[i] != '~')
		{
			fprintf(get_data, "%lf\n", data[i]);
		}
	}

	fclose(get_data);
	fflush(stdout);
	return NO_ERROR;
}

