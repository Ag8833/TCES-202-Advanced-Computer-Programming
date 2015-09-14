/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
file.h

Brandon Watt & Andrew Gates

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifndef FILE_H
#define FILE_H

#define MAX_FILE_NAME_LEN 		101

#define FILENAME_FIELD 				0
#define CREATION_DATE_FIELD 		1
#define MODIFICATION_DATE_FIELD 	2
#define OWNER_FIELD 				3
#define NUMBER_RECORDS_FIELD 		4
#define STATE_FIELD 				5
#define MEAN_FIELD					6
#define MEDIAN_FIELD				7
#define VARIANCE_FIELD				8
#define STD_DEV_FIELD				9

// prototypes
int file_main_menu (FILE *);
int file_menu(void);
int file_create(FILE *);
int file_open(FILE *);
int file_rename(void);
int file_copy(void);
int file_print(void);
int file_delete(void);

int print_file_menu(void);
void init_file_header (FileHeaderPtr, char *);
int get_file_header (FileHeaderPtr, FILE *);
void parse_file_header(FileHeaderPtr fh_p, char * header);
char * to_string_file_header (FileHeaderPtr fh_p, char * buff);
int check_file_name(const char *);

#endif
