#include "loader_params.h"
#include <stdio.h>
#include <stddef.h>

#define OFFSET(x, str, mem) printf("#define " #x " %d\n",(int) offsetof(str, mem))
#define SIZE(x, str) printf("#define " #x " %d\n", (int) sizeof(str))
#define HEADER_START() printf("#ifndef ASM_OFFSET_H\n#define ASM_OFFSET_H\n")
#define HEADER_END() printf("#endif\n")

int main(int argc, char *argv[]) {
	
	HEADER_START();

	OFFSET(PARAMS_unmap_address_start, LOADER_PARAMS, unmap_address_start);
	OFFSET(PARAMS_unmap_address_size, LOADER_PARAMS, unmap_address_size);

	OFFSET(PARAMS_number_of_sections, LOADER_PARAMS, number_of_sections);
	OFFSET(PARAMS_section_data_start, LOADER_PARAMS, section_data_start);

	OFFSET(PARAMS_final_unmap_start, LOADER_PARAMS, final_unmap_start);
	OFFSET(PARAMS_final_unmap_size, LOADER_PARAMS, final_unmap_size);

	OFFSET(SECTION_start_address, SECTION_PARAMS, start_address);
	OFFSET(SECTION_size, SECTION_PARAMS, size);
	OFFSET(SECTION_proto, SECTION_PARAMS, proto);
	OFFSET(SECTION_flags, SECTION_PARAMS, flags);
	OFFSET(SECTION_filename, SECTION_PARAMS, filename[0]);
	OFFSET(SECTION_open_flags, SECTION_PARAMS, open_flags);
	OFFSET(SECTION_offset, SECTION_PARAMS, offset);
	OFFSET(SECTION_is_raw, SECTION_PARAMS, is_raw);

	SIZE(SECTION_SIZE, SECTION_PARAMS);

	HEADER_END();

	return 0;
}
