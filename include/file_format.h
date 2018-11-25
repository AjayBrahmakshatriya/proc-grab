#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <stdio.h>
#include "proc_fs.h"

typedef struct __attribute__((packed)) {
	int number_of_sections;
}FILE_HEADER;

typedef enum {
	raw = 0,
	file_mapped = 1,
}SECTION_TYPE;

typedef struct __attribute__((packed)) {
	unsigned long long lower;
	unsigned long long upper;
	SECTION_PERMS perms;	
	SECTION_TYPE type;
	char filename[128];
	unsigned long long raw_data_offset;
}SECTION_FILE_HEADER;

typedef struct {
	char filename[128];
	FILE* file_handle;
	FILE_HEADER header;
	int for_writing;
}FILE_HANDLE;


FILE_HANDLE* create_file(char *file_name);
void close_file(FILE_HANDLE* handle);
void write_section(FILE_HANDLE *handle, SECTION_FILE_HEADER *header, char *raw_data);
FILE_HANDLE* open_file(char *file_name);
void read_section(FILE_HANDLE *handle, SECTION_FILE_HEADER *header);
#endif
