#include "ft_nm.h"

int check_shdr(void *sh, fdata_t *fdata) {
	Elf64_Shdr	*sh64 = (Elf64_Shdr*)sh;
	Elf32_Shdr	*sh32 = (Elf32_Shdr*)sh;
	size_t		sh_name;
	size_t		sh_size;
	size_t		sh_offset;
	size_t		sh_link;

	if (fdata->class == 1) {
		sh_name = sh32->sh_name;
		sh_size = sh32->sh_size;
		sh_offset = sh32->sh_offset;
		sh_link = sh32->sh_link;
	}
	else {
		sh_name = sh64->sh_name;
		sh_size = sh64->sh_size;
		sh_offset = sh64->sh_offset;
		sh_link = sh64->sh_link;
	}

	//check section offset
	if (sh_offset > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section size
	if (sh_offset + sh_size > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section link
	if (sh_link > fdata->shtab_count) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	return (0);
}