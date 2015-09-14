/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
file.c

Brandon Watt & Andrew Gates
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "global.h"
#include "screen_utils.h"
#include "file.h"

/* Called from the main menu, this menu gets the user's selections for how to work with the files.
 * Calls the file_menu to get a valid selection. Based on the selection,
 * calls the appropriate file functions.
 */
int file_main_menu(FILE * fid)
{
	int choice = 0;

	while (choice != EXIT_FILE_SEL)
	{
		choice = file_menu();
		switch (choice)
		{
		case CREATE_SEL:
		{
			printf("File Creation:\n");
			file_create(fid);
			break;
		}
		case OPEN_SEL:
		{
			file_open(fid);
			break;
		}
		case RENAME_SEL:
		{
			file_rename();
			break;
		}
		case COPY_SEL:
		{
			file_copy();
			break;
		}
		case PRINT_SEL:
		{
			file_print();
			break;
		}
		case DELETE_SEL:
		{
			file_delete();
			break;
		}
		case EXIT_FILE_SEL:
		{
			clear_screen();
			break;
		}
		}

	}
	return 0;
}

/* gets the selection from the file menu. It calls print_file_menu() to put the menu on the screen.
 * If the user enters an invalid selection they are given opportunities to get it right.
 */
int file_menu(void) {
	// Phase 1
	char sel_s[3];
	int sel;

	print_file_menu();
	gets(sel_s);
	sel = atoi(sel_s);
	if (sel < CREATE_SEL || sel > EXIT_FILE_SEL) {
		while (1) {
			clear_screen();
			printf("Incorrect selection. Please enter one of these choices.\n");
			print_file_menu();
			gets(sel_s);
			sel = atoi(sel_s);
			if (!(sel < CREATE_SEL || sel > EXIT_FILE_SEL))
				break;
		}
	}
	return sel;
}

/* prints the file menu options to the screen. Is called from multiple places.
 * "File Menu: 1) New 2) Open 3) Rename 4) Copy 5) Print 6) Delete 7) Exit\n")
 */
int print_file_menu(void) {
	printf( "Statpac File menu options.\n"
			"File Menu: 1) New 2) Open 3) Rename 4) Copy 5) Print 6) Delete 7) Exit\n"
			"Please choose an option: ");

	return 0;
}

/************************************************************************************************
 Menu-called Functions
 These are the functions that implement the menu options for the file management system

 ************************************************************************************************/

/* gets user input for new file name, checks to make sure no file of that type already
 exists, if it doesn't exist it creates the file and initializes the header lines (see below)
 Returns file handle in fid.
 */
int file_create(FILE * fid) {
	// Phase 1
	char file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to create: ");
	gets(file_name);
	puts(file_name);

	int check = check_file_name(file_name);
	if (check != FILE_NAME_FORMAT_ERROR)
	{
		fid = fopen(file_name, "w");
		if (fid != NULL)
		{
			// Allocate and Initialize the FileHeader structure.
			FileHeaderPtr blank_ptr = malloc(sizeof(struct file_header));
			init_file_header(blank_ptr, file_name);

			// Put FileHeader into file.
			char *buffer = calloc(256, sizeof(char));
			sprintf(buffer, "%s,%s,%s,%s,%d,%s,%f,%f,%f,%f\n",
					blank_ptr->filename, blank_ptr->creation_date,
					blank_ptr->modification_date, blank_ptr->owner,
					blank_ptr->num_records, "new", blank_ptr->mean,
					blank_ptr->median, blank_ptr->variance, blank_ptr->std_dev);

			fputs(buffer, fid);

			printf("File was successfully created.\n");
			fclose(fid);
		}
	}
	press_enter();
	clear_screen();

	return 0;
}

/* Opens an existing file for use. Returns successfully opened file in fid */
int file_open(FILE * fid) {
	// Phase 1
	char file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to open: ");
	gets(file_name);
	printf("\n");

	fid = fopen(file_name, "r");
	if (fid == NULL)
	{
		printf("File error: Check that the file is created.\n");
	}
	else
	{
		FileHeaderPtr blank_ptr = malloc(sizeof(struct file_header));
		get_file_header(blank_ptr, fid);

		// Prints FileHeader out to console.
		char *buffer = calloc(256, sizeof(char));
		printf("%s", to_string_file_header(blank_ptr, buffer));


		press_enter();
		clear_screen();
		printf("File was opened successfully.\n");
	}
	press_enter();
	clear_screen();

	return 0;
}

/* allows the user to change the name of an existing file to an alternate
 */
int file_rename(void) {
	// Phase 1
	char old_file_name[MAX_FILE_NAME_LEN];
	char new_file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to rename: ");
	gets(old_file_name);

	FILE *test = fopen(old_file_name, "r");
	int check = check_file_name(old_file_name);
	if(check != FILE_NAME_FORMAT_ERROR)
	{
		if (test == NULL )
		{
			printf("\nError in opening %s: Check to make sure file name is correct and file exists.",
					old_file_name);
			printf("\nFile Error: check that the file exists.\n");
		}
		else
		{
			fclose(test);
			printf("Enter the new name of the file: ");
			gets(new_file_name);

			rename(old_file_name, new_file_name);

			printf("\nFile was successfully renamed.\n");
		}
	}
	press_enter();
	clear_screen();

	return 0;
}

/* makes a copy of an existing file. The new file must have a different legal filename
 */
int file_copy(void) {
	// Phase 1
	char file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to copy: ");
	gets(file_name);

	// Get the original file.
	FILE *original = fopen(file_name, "r");

	int check = check_file_name(file_name);

	char file_name2[MAX_FILE_NAME_LEN];

	if (check != FILE_NAME_FORMAT_ERROR)
	{
		printf("Enter the name of the new file copy: ");
		gets(file_name2);

		if (original == NULL)
		{
				printf("\nError in opening %s: Check to make sure file name is correct and file exists.",
						file_name);
				printf("\nFile Error: check that the file exists.\n");
		}
		else
		{

			FileHeaderPtr blank_ptr = (FileHeaderPtr) malloc(sizeof(FileHeader));
			get_file_header(blank_ptr, original);

			// Put information into new file.
			char *buffer = calloc(256, sizeof(char));
			sprintf(buffer, "%s,%s,%s,%s,%d,%s,%f,%f,%f,%f", blank_ptr->filename,
					blank_ptr->creation_date, blank_ptr->modification_date,
					blank_ptr->owner, blank_ptr->num_records, "new",
					blank_ptr->mean, blank_ptr->median, blank_ptr->variance,
					blank_ptr->std_dev);

			FILE *copy = fopen(file_name2, "w");

			fputs(buffer, copy);
			printf("File successfully copied.\n");
			fclose(original);
			fclose(copy);
		}

	}
	press_enter();
	clear_screen();
	return 0;
}

/* copies a data file to a formatted version suitable for printing on a printer. This is a preliminary
 version. See to_string_file_header() below for an alternate method.
 */
int file_print(void) {
	// Phase 1
	char file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to print: ");
	gets(file_name);

	// Get the original file.
	FILE *original = fopen(file_name, "r");

	int check = check_file_name(file_name);

	char file_name2[MAX_FILE_NAME_LEN];

	if (check != FILE_NAME_FORMAT_ERROR)
	{
		printf("Enter the name of the new file copy: ");
		gets(file_name2);
		if (original == NULL)
		{
			printf("\nError in opening %s: Check to make sure file name is correct and file exists.", file_name);
		}
		else
		{
			FileHeaderPtr blank_ptr = (FileHeaderPtr) malloc(sizeof(struct file_header));

			char header[1000];
			fgets(header, 999, original);
			parse_file_header(blank_ptr, header);

			// Put information into new file.
			char *buffer = calloc(256, sizeof(char));
			char *temp = to_string_file_header(blank_ptr, buffer);

			FILE *print = fopen(file_name2, "w");

			fputs(buffer, print);
			fclose(original);
			fclose(print);

			printf("File successfully printed.\n");
		}
	}
	press_enter();
	clear_screen();
	return 0;
}

/* allows the user to delete an existing file; gives the user a second chance to abort, uses the remove() function from stdio.h
 */
int file_delete(void)
{
	// Phase 1
	char file_name[MAX_FILE_NAME_LEN];

	printf("Enter the name of the file you want to delete: ");
	gets(file_name);

	int check = check_file_name(file_name);

	if (check != FILE_NAME_FORMAT_ERROR)
	{
		if (unlink(file_name) == 0)
		{
			printf("The file was successfully deleted.\n");
		}
		else
		{
			printf("File Error: check that the file exists.\n");
		}
	}

	press_enter();
	clear_screen();
	return 0;
}

/***************************************************************************************************
 Utilities
 Helper functions that support the file management system (menu)
 ***************************************************************************************************/
/* fhp should have already been allocated so that it contains a valid heap pointer. filename space will need to be allocated here.
 * Date spaces were allocated in the structure. Owner will need to be determined
 */
void init_file_header(FileHeaderPtr fhp, char * name)
{

	char date[26];
	char owner[MAX_OWNER_FIELD];

	fhp->filename = (char *) malloc(sizeof(char) * strlen(name) + 1);
	strcpy(fhp->filename, name);

	// this algorithm used to initialize a file header record
	//when a file is first created
	time_t timer;
	struct tm *timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	strcpy(date, (char *) asctime(timeinfo));
	date[24] = 0;

	strcpy(fhp->creation_date, date);
	strcpy(fhp->modification_date, date);

	fhp->num_records = 0;
	fhp->state = NEW;
	fhp->mean = 0.0L;
	fhp->median = 0.0L;
	fhp->variance = 0.0L;
	fhp->std_dev = 0.0L;

	printf("Enter owner name: ");
	gets(owner);
	printf("\n");
	fhp->owner = (char *) malloc(sizeof(char) * strlen(owner) + 1);
	strcpy(fhp->owner, owner);
}

/* gets a file header record from the file f_p and parses it into the structure fh_p.
 * When it returns fh_p will be pointing to a filled file header record.
 */
int get_file_header(FileHeaderPtr fh_p, FILE * f_p)
{
	if (!f_p)
		return FILE_ERROR;

	char header[1000];

	if (!fh_p)
		fh_p = (FileHeaderPtr) malloc(sizeof(FileHeader));

	fgets(header, 999, f_p);	// reads the header record
	puts(header);
	parse_file_header(fh_p, header);
	return NO_ERROR;
}

/* this function parses a single string, header, containing a file header record and puts the values into
 * the fields of the structure pointed to by fh_p. Both fh_p and header must be valid pointers.
 */
void parse_file_header(FileHeaderPtr fh_p, char * header) {
	char buff[256];
	char * pos;
	int i, field;

	pos = strchr(header, ',');
	*pos++ = 0;
	strcpy(buff, header);	// copies filename into buff
	i = strlen(buff);

	fh_p->filename = (char *) malloc(sizeof(char) * i + 1); // allocate space for the name
	strcpy(fh_p->filename, buff);

	header = pos;		// resets head of header string
	for (field = 1; field < HEADER_FIELDS; field++) {
		switch (field) {
		case CREATION_DATE_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(fh_p->creation_date, header);
			header = pos;
			break;
		}
		case MODIFICATION_DATE_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(fh_p->modification_date, header);
			header = pos;
			break;
		}
		case OWNER_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);	//
			i = strlen(buff);
			fh_p->owner = (char *) malloc(sizeof(char) * i + 1);
			strcpy(fh_p->owner, buff);
			header = pos;
			break;
		}
		case NUMBER_RECORDS_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);
			fh_p->num_records = atoi(buff);
			header = pos;
			break;
		}
		case STATE_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);
			if (strncmp(buff, "new", 3) == 0) { // a use of goto to emulate a switch statement with non-int
				fh_p->state = NEW;
			} else if (strncmp(buff, "entry", 5) == 0) {
				fh_p->state = ENTRY;
			} else if (strncmp(buff, "sorted", 6) == 0) {
				fh_p->state = SORTED;
			}
			header = pos;
			break;
		}
		case MEAN_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);
			fh_p->mean = atof(buff);
			header = pos;
			break;
		}
		case MEDIAN_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);
			fh_p->median = atof(buff);
			header = pos;
			break;
		}
		case VARIANCE_FIELD: {
			pos = strchr(header, ',');
			*pos++ = 0;
			strcpy(buff, header);
			fh_p->variance = atof(buff);
			header = pos;
			break;
		}
		case STD_DEV_FIELD: {
			strcpy(buff, header);
			fh_p->std_dev = atof(buff);
			// puts(buff);
			break;
		}
		}	// end switch
	}	// end for loop
}

/* creates a string containing all of the fields from a file header structure pointed at by fh_p.
 * The char string buff should have been pre-allocated from the calling function.
 */
char * to_string_file_header(FileHeaderPtr fh_p, char * buff) {

	char loc_buff[11];

	if (fh_p->state == NEW)
		strcpy(loc_buff, "new");
	else if (fh_p->state == ENTRY)
		strcpy(loc_buff, "entry");
	else if (fh_p->state == SORTED)
		strcpy(loc_buff, "sorted");

	sprintf(buff,
			"File: %s\nCreated: %s\nModified: %s\nOwner: %s\nRecords: %d\nState: %s\nMean: %f\nMedian: %f\nVariance: %f\nStd. Dev: %f\n",
			fh_p->filename, fh_p->creation_date, fh_p->modification_date,
			fh_p->owner, fh_p->num_records, loc_buff, fh_p->mean, fh_p->median,
			fh_p->variance, fh_p->std_dev);
	return buff;
}

/* checks the file name for erroneous characters - this is more restrictive than Unix file naming conventions,
 * but the rules are application specific.
 */
int check_file_name(const char * filename) {
	int i;
	int length = strlen(filename);

	for (i = 0; i < length; i++) {
		if (!(isalnum(filename[i]) || filename[i] == '_' || filename[i] == '.'))
		{
			printf("\nIncorrect file name format.\n");
			printf("File Error: check that the file exists.\n");
			return FILE_NAME_FORMAT_ERROR;
		}
	}
	if (i == length)
		return NO_ERROR;
}
