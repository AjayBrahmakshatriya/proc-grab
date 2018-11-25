#include "file_format.h"
#include "logs.h"
#include "string.h"
#include <stdlib.h>
void write_file_header(FILE_HANDLE *handle) {
	unsigned long offset = ftell(handle->file_handle);
	fseek(handle->file_handle, 0, SEEK_SET);
	fwrite(&handle->header, sizeof(handle->header), 1, handle->file_handle);
	fseek(handle->file_handle, offset, SEEK_SET);
}

FILE_HANDLE* create_file(char *file_name) {
	FILE *file_handle = fopen(file_name, "w");
	if (file_handle == NULL) {
		err("Cannot open file: %s for opening\n", file_name);
		return NULL;
	}
	FILE_HANDLE *handle = malloc(sizeof * handle);
	strcpy(handle->filename, file_name);
	handle->file_handle = file_handle;
	handle->header.number_of_sections = 0;
	fseek(handle->file_handle, sizeof(handle->header), SEEK_SET);

	write_file_header(handle);
	handle->for_writing = 1;
	return handle;
}

void close_file(FILE_HANDLE *handle) {
	fclose(handle->file_handle);
	free(handle);
}
void write_section(FILE_HANDLE *handle, SECTION_FILE_HEADER *header, char *raw_data) {
	if(header->type == raw && header->upper != header->lower)
		header->raw_data_offset = ftell(handle->file_handle) + sizeof(*header);
	fwrite(header, sizeof(*header), 1, handle->file_handle);
	if(header->type == raw && header->upper != header->lower)
		fwrite(raw_data, header->upper - header->lower, 1, handle->file_handle);
	handle->header.number_of_sections++;
	write_file_header(handle);
}
FILE_HANDLE* open_file(char *file_name) {
	FILE *file_handle = fopen(file_name, "r");
	if (file_handle == NULL) {
		err("Cannot open file: %s for reading\n", file_name);
		return NULL;
	}
	FILE_HANDLE *handle = malloc(sizeof * handle);
	strcpy(handle->filename, file_name);
	handle->file_handle = file_handle;
	fread(&handle->header, sizeof(handle->header), 1, handle->file_handle);
	handle->for_writing = 0;
	return handle;
}
void read_section(FILE_HANDLE *handle, SECTION_FILE_HEADER *header) {
	fread(header, sizeof(*header), 1, handle->file_handle);
	if(header->type == raw && header->upper != header->lower)
		fseek(handle->file_handle, header->upper - header->lower, SEEK_CUR);
	
}
