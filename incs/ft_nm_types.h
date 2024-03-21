#ifndef FT_NM_TYPES_H
#define FT_NM_TYPES_H

#include <elf.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ELF_ST_TYPE(val)	((val) & 0xf)
#define ELF_ST_BIND(val)	((val) >> 4)

typedef struct symbol_s {
	size_t	value;
	uint8_t	type;
	char	*name;
	uint8_t	info;
} symbol_t;

typedef struct symlist_s {
	symbol_t			sym;
	struct symlist_s	*next;
} symlist_t;

typedef struct shtab_s {
	size_t	off;					//section header table offset
	size_t	entsize;				//section header table entry size
	size_t	count;					//section header table entry count
	size_t	sh_stidx;				//section header string table index
} shtab_t;

typedef struct fdata_s {
	int16_t		fd;					//file descriptor
	char		*name;				//file name
	struct stat	st;					//file stats
	void		*map;				//file mapping in memory
	uint8_t		class; 				//os class 32bit or 64bit
	shtab_t		shtab;				//section header table
	symlist_t	*symlist;			//symbol list
} fdata_t;

typedef struct data_s {
	uint8_t	opt;
	fdata_t	fdata;
} data_t;

#endif // FT_NM_TYPES_H
