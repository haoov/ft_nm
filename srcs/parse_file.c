#include "ft_nm.h"

int check_format(int fd, char *file) {
	char e_ident[EI_NIDENT];
	if (read(fd, e_ident, EI_NIDENT) != EI_NIDENT) {
		SET_ERROR(ERRNO, file);
		return (-1);
	}
	if (ft_strncmp(e_ident, ELFMAG, SELFMAG)) {
		SET_ERROR(ERWFFMT, file);
		return (-1);
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
	if (check_format(fdata->fd, file) == -1)
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

	//testing
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)fdata->map;
	Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff);
	char **strtab;
	size_t size;
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_type == SHT_STRTAB) {
			strtab = (char **)((char *)fdata->map + shdr[i].sh_offset);
			size = shdr[i].sh_size;
			for (size_t j = 0; j < size; j++) {
				if (strtab[j] == NULL)
					break;
				ft_printf(1, "%s\n", strtab[j]);
			}
			break;
		}
	}
	return (0);
}