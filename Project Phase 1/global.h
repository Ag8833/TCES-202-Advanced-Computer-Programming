/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
global.h

Brandon Watt & Andrew Gates
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef GLOBAL_H
#define GLOBAL_H

#define TRUE 1
#define FALSE 0

#define NO_ERROR 0
#define FILE_ERROR 1
#define FILE_NAME_FORMAT_ERROR 2

/* main menu selections
*/
#define FILE_SEL 1
#define EDIT_SEL 2
#define SEARCH_SEL 3
#define COMPUTE_SEL 4
#define REPORT_SEL 5
#define EXIT_SEL 6

/* File menu selections
*/
#define CREATE_SEL 1
#define OPEN_SEL 2
#define RENAME_SEL 3
#define COPY_SEL 4
#define PRINT_SEL 5
#define DELETE_SEL 6
#define EXIT_FILE_SEL 7

/* Edit menu selections
*/
#define EDIT_ADD_SEL 1
#define EDIT_EDIT_SEL 2
#define EDIT_DELETE_SEL 3

#define MAX_DATE_FIELD 25
#define MAX_NAME_FIELD 65
#define MAX_OWNER_FIELD 65

typedef enum{NEW, ENTRY, SORTED} State ;

#define HEADER_FIELDS 10

typedef struct file_header {
	char * filename;
	char creation_date[25];
	char modification_date[25];
	char * owner;
	long num_records;
	State state;
	double mean;
	double median;
	double variance;
	double std_dev;
} FileHeader;

typedef FileHeader *FileHeaderPtr;

typedef struct data_record {
	char entry_date[25];
	char mod_date[25];
	double data;
} DataRecord;

typedef DataRecord *DataRecordPtr;

#endif
