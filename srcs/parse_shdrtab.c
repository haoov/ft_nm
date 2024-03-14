#include "ft_nm.h"

int parse_shdrtab_32(fdata_t *fdata) {
	Elf32_Shdr	*shstrtab;
	Elf32_Shdr	*shcur;
	size_t		shcur_off;

	for (int i = 0; i < fdata->shtab_count; i++) {
		//get current section header
		shcur_off = fdata->shtab_off + i * fdata->shtab_entsize;
		shcur = (Elf32_Shdr*)((byte_t*)fdata->map + shcur_off);
		
		//check section header
		if (check_shdr(shcur, fdata) == -1)
			return (-1);

		//check section type
		if (shcur->sh_type == SHT_SYMTAB) {
			if (parse_symtab_32(fdata, shcur) == -1)
				return (-1);
		}
	}
}

int parse_shdrtab_64(fdata_t *fdata) {
	Elf64_Shdr	*shstrtab;
	Elf64_Shdr	*shcur;
	size_t		shcur_off;

	for (int i = 0; i < fdata->shtab_count; i++) {
		//get current section header
		shcur_off = fdata->shtab_off + i * fdata->shtab_entsize;
		shcur = (Elf64_Shdr*)((byte_t*)fdata->map + shcur_off);
		
		//check section header
		if (check_shdr(shcur, fdata) == -1)
			return (-1);

		//check section type
		if (shcur->sh_type == SHT_SYMTAB) {
			if (parse_symtab_64(fdata, shcur) == -1)
				return (-1);
		}
	}

	return (0);
}