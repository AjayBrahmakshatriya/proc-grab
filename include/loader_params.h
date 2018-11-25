#ifndef LOADER_PARAMS_H
#define LOADER_PARAMS_H



typedef struct {	
	unsigned long long unmap_address_start;
	unsigned long long unmap_address_size;
	

	unsigned long long number_of_sections;
	unsigned long long section_data_start;

	unsigned long long final_unmap_start;
	unsigned long long final_unmap_size;

}LOADER_PARAMS;


typedef struct {
	unsigned long long start_address;
	unsigned long long size;
	unsigned long long proto;
	unsigned long long flags;
	char filename[128];
	unsigned long long open_flags;
	unsigned long long offset;
	unsigned long long is_raw;
}SECTION_PARAMS;



#endif
