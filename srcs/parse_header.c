#include "ft_nm.h"

int parse_header(fdata_t *fdata) {
	size_t ehdr_size;
	size_t size;
	size_t shstrtab_idx;

	if (fdata->class == 1) {
		ehdr_size = ((Elf32_Ehdr *)fdata->map)->e_ehsize;
		size = sizeof(Elf32_Ehdr);
		fdata->shtab_off = ((Elf32_Ehdr *)fdata->map)->e_shoff;
		fdata->shtab_entsize = ((Elf32_Ehdr *)fdata->map)->e_shentsize;
		fdata->shtab_count = ((Elf32_Ehdr *)fdata->map)->e_shnum;
		shstrtab_idx = ((Elf32_Ehdr *)fdata->map)->e_shstrndx;
	}
	else {
		ehdr_size = ((Elf64_Ehdr *)fdata->map)->e_ehsize;
		size = sizeof(Elf64_Ehdr);
		fdata->shtab_off = ((Elf64_Ehdr *)fdata->map)->e_shoff;
		fdata->shtab_entsize = ((Elf64_Ehdr *)fdata->map)->e_shentsize;
		fdata->shtab_count = ((Elf64_Ehdr *)fdata->map)->e_shnum;
		shstrtab_idx = ((Elf64_Ehdr *)fdata->map)->e_shstrndx;
	}

	//check header size
	if (ehdr_size != size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header table offset
	if (fdata->shtab_off == 0 || fdata->shtab_off > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header table size
	size_t shdtab_size = fdata->shtab_count * fdata->shtab_entsize;
	if (fdata->shtab_off + shdtab_size > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header string table index
	if (shstrtab_idx == SHN_UNDEF || shstrtab_idx >= fdata->shtab_count) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//get section header string table offset
	fdata->shstrtab_off = fdata->shtab_off + shstrtab_idx * fdata->shtab_entsize;

	return (0);
}