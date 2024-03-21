#include "ft_nm.h"

int parse_header(fdata_t *fdata) {
	size_t ehdr_size;
	size_t size;

	if (fdata->class == 1) {
		ehdr_size = ((Elf32_Ehdr *)fdata->map)->e_ehsize;
		size = sizeof(Elf32_Ehdr);
		fdata->shtab.off = ((Elf32_Ehdr *)fdata->map)->e_shoff;
		fdata->shtab.entsize = ((Elf32_Ehdr *)fdata->map)->e_shentsize;
		fdata->shtab.count = ((Elf32_Ehdr *)fdata->map)->e_shnum;
		fdata->shtab.sh_stidx = ((Elf32_Ehdr *)fdata->map)->e_shstrndx;
	}
	else {
		ehdr_size = ((Elf64_Ehdr *)fdata->map)->e_ehsize;
		size = sizeof(Elf64_Ehdr);
		fdata->shtab.off = ((Elf64_Ehdr *)fdata->map)->e_shoff;
		fdata->shtab.entsize = ((Elf64_Ehdr *)fdata->map)->e_shentsize;
		fdata->shtab.count = ((Elf64_Ehdr *)fdata->map)->e_shnum;
		fdata->shtab.sh_stidx = ((Elf64_Ehdr *)fdata->map)->e_shstrndx;
	}

	//check header size
	if (ehdr_size != size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header table offset
	if (fdata->shtab.off == 0 || fdata->shtab.off > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header table size
	size_t shdtab_size = fdata->shtab.count * fdata->shtab.entsize;
	if (fdata->shtab.off + shdtab_size > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header string table index
	if (fdata->shtab.sh_stidx == SHN_UNDEF || fdata->shtab.sh_stidx >= fdata->shtab.count) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	return (0);
}
