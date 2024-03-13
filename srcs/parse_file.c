#include "ft_nm.h"

int parse_symlist(fdata_t *fdata, Elf64_Ehdr *ehdr, Elf64_Shdr *shdr, Elf64_Shdr *sh_shstrtab) {
	Elf64_Shdr		*sh_strtab;
	Elf64_Shdr		*symshdr;
	char			*strtab;
	__SIZE_TYPE__	idx;
	int 			j;
	__SIZE_TYPE__	pos = 0;

	for (int i = 0; i < (int)(shdr->sh_size / shdr->sh_entsize); i++) {
		Elf64_Sym *sym = (Elf64_Sym *)((char *)fdata->map + shdr->sh_offset + i * shdr->sh_entsize);
		if ((sym->st_info & 0x0F) == STT_SECTION) {
			symshdr = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff + sym->st_shndx * ehdr->e_shentsize);
			strtab = (char *)fdata->map + sh_shstrtab->sh_offset;
			idx = symshdr->sh_name;
		}
		else {
			sh_strtab = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff + shdr->sh_link * ehdr->e_shentsize);
			strtab = (char *)fdata->map + sh_strtab->sh_offset;
			idx = sym->st_name;
		}
		j = 0;
		while (j < idx) {
			pos += ft_strlen(strtab + pos);
			j++;
		}
		if ((sym->st_info & 0x0F) == STT_NOTYPE && (sym->st_info >> 4) == STB_LOCAL)
			continue;
		ft_printf(1, "%.16x %c %s\n", sym->st_value, 'T', strtab + idx + pos);
	}
	return (0);
}

int parse_file(char *file, data_t *data) {
	fdata_t		*fdata = &data->fdata;

	fdata->name = file;

	//open file
	fdata->fd = open(file, O_RDONLY);
	if (fdata->fd == -1) {
		if (errno == ENOENT)
			SET_ERROR(ERRNO);
		return (-1);
	}

	//check file stats
	if (fstat(fdata->fd, &fdata->st) == -1) {
		SET_ERROR(ERRNO);
		return (-1);
	}
	if (S_ISDIR(fdata->st.st_mode)) {
		SET_ERROR(EISDIR);
		return (-1);
	}

	//check file size
	if (fdata->st.st_size == 0) {
		SET_ERROR(EREMPTY);
		return (-1);
	}
	if (fdata->st.st_size < 84 || fdata->st.st_size < 128) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//map file in memory
	fdata->map = mmap(0, fdata->st.st_size, PROT_READ, MAP_PRIVATE, fdata->fd, 0);
	if (fdata->map == MAP_FAILED) {
		SET_ERROR(ERRNO);
		return (-1);
	}

	//check file format
	if (ft_strncmp((char*)fdata->map, ELFMAG, SELFMAG) != 0) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check file os type
	byte_t ost = *((byte_t*)fdata->map + 4);
	ft_printf(1, "ost: %d\n", ost);
	if (ost > 2) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	if (ost == 1)
		parse_symbols_32(fdata);
	else
		parse_symbols_64(fdata);
	
	//find section header string table
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)fdata->map;
	Elf64_Shdr *sh_shstrtab = NULL;
	__SIZE_TYPE__ sh_shstrtab_offset = ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize;
	sh_shstrtab = (Elf64_Shdr *)((char *)fdata->map + sh_shstrtab_offset);

	//find symbol tables
	for (int i = 0; i < ehdr->e_shnum; i++) {
		Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff + i * ehdr->e_shentsize);
		if (shdr->sh_type == SHT_SYMTAB) {
			if (parse_symlist(fdata, ehdr, shdr, sh_shstrtab) == -1)
				return (-1);
		}
	}
	return (0);
}

int parse_symbols_32(fdata_t *fdata) {

	//cast elf header
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)fdata->map;

	//check header size
	if (ehdr->e_ehsize != sizeof(Elf32_Ehdr)) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//check section header table offset
	if (ehdr->e_shoff == 0 || ehdr->e_shoff > fdata->st.st_size) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}
}