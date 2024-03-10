#ifndef FT_NM_TYPES_H
#define FT_NM_TYPES_H

#include <elf.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct fdata_s {
	int			fd;
	char		*name;
	struct stat	st;
	void		*map;
} fdata_t;

typedef struct data_s {
	uint8_t	opt;
	fdata_t	fdata;
} data_t;

#endif // FT_NM_TYPES_H