#ifndef FT_NM
#define FT_NM

#include <elf.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "../libft/incs/libft.h"
#include "ft_nm_types.h"
#include "nm_error.h"

#define OPTS	"agnurp"

#define OALL	0x01 //-a display all symbol table entries
#define OGLOB	0x02 //-g display only global symbols
#define ONUM	0x04 //-n sort numerically
#define OUND	0x08 //-u display only undefined symbols
#define OREV	0x10 //-r display in reverse order
#define ONSRT	0x20 //-p dont sort

typedef uint8_t	byte_t;

int		parse_opt(data_t *data, char **argv);
int		parse_file(char *file, data_t *data);
int		parse_header(fdata_t *fdata);
int		parse_shdrtab_32(fdata_t *fdata);
int		parse_shdrtab_64(fdata_t *fdata);
int		parse_symtab_32(fdata_t *fdata, Elf32_Shdr *shsymtab);
int		parse_symtab_64(fdata_t *fdata, Elf64_Shdr *shsymtab);

int		check_shdr(void *sh, fdata_t *fdata);

#endif // FT_NM
