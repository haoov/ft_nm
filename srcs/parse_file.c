#include "ft_nm.h"

int check_format(fdata_t fdata) {
	char e_ident[EI_NIDENT];
	if (read(fdata.fd, e_ident, EI_NIDENT) != EI_NIDENT) {
		SET_ERROR(ERRNO, fdata.name);
		return (-1);
	}
	if (ft_strncmp(e_ident, ELFMAG, SELFMAG)) {
		SET_ERROR(ERWFFMT, fdata.name);
		return (-1);
	}
	return (0);
}

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
			SET_ERROR(ERNOFILE, file);
		else
			SET_ERROR(ERRNO, file);
		return (-1);
	}

	//check file format
	if (check_format(*fdata) == -1)
		return (-1);

	//get file stats
	if (fstat(fdata->fd, &fdata->st) == -1) {
		SET_ERROR(ERRNO, file);
		return (-1);
	}

	//map file in memory
	fdata->map = mmap(0, fdata->st.st_size, PROT_READ, MAP_PRIVATE, fdata->fd, 0);
	if (fdata->map == MAP_FAILED) {
		SET_ERROR(ERRNO, file);
		return (-1);
	}
	
	//find section header string table
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)fdata->map;
	Elf64_Shdr *sh_shstrtab = NULL;
	__SIZE_TYPE__ sh_shstrtab_offset = ehdr->e_shoff + ehdr->e_shstrndx * ehdr->e_shentsize;
	sh_shstrtab = (Elf64_Shdr *)((char *)fdata->map + sh_shstrtab_offset);

	//find symbol tables
	for (int i = 0; i < ehdr->e_shnum; i++) {
		Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff + i * ehdr->e_shentsize);
		if (shdr->sh_type == SHT_SYMTAB || shdr->sh_type == SHT_DYNSYM) {
			if (parse_symlist(fdata, ehdr, shdr, sh_shstrtab) == -1)
				return (-1);
		}
	}
	return (0);
}