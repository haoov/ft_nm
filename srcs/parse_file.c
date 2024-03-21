#include "ft_nm.h"

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
		SET_ERROR(ERISDIR);
		return (-1);
	}
	if (S_ISDIR(fdata->st.st_mode)) {
		SET_ERROR(ERISDIR);
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

	//check file class
	fdata->class = *((byte_t*)fdata->map + 4);
	if (fdata->class == 0 || fdata->class > 2) {
		SET_ERROR(ERWFFMT);
		return (-1);
	}

	//parse header
	if (parse_header(fdata) == -1)
		return (-1);

	if (fdata->class == ELFCLASS32) {
		if (parse_shdrtab_32(data, fdata))
			return (-1);
	}
	else {
		if (parse_shdrtab_64(data, fdata))
			return (-1);
	}

	return (0);
}
