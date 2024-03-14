#ifndef FT_NM_TYPES_H
#define FT_NM_TYPES_H

#include <elf.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct symbol_s {
	size_t	value;
	uint8_t	info;
	char	*name;
} symbol_t;

typedef struct symlist_s {
	symbol_t			*sym;
	struct symlist_s	*next;
} symlist_t;

typedef struct fdata_s {
	int16_t		fd;
	char		*name;
	struct stat	st;
	void		*map;
	uint8_t		class;
	size_t		shtab_off;
	size_t		shtab_entsize;
	size_t		shtab_count;
	size_t		shstrtab_off;
	symlist_t	*symlist;
} fdata_t;

typedef struct data_s {
	uint8_t	opt;
	fdata_t	fdata;
} data_t;

#endif // FT_NM_TYPES_H