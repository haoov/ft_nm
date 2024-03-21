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

#define ULCMP	1
#define STRCMP	2

typedef uint8_t	byte_t;

#define SHDR_OFF(fdata, idx)	(fdata->shtab.off + idx * fdata->shtab.entsize)
#define SHDR(fdata, idx)		((byte_t*)fdata->map + SHDR_OFF(fdata, idx))
#define STRTAB(fdata, sh)		((char*)((byte_t*)fdata->map + sh->sh_offset))

#define RAWSEC(s)		(s->sh_type == SHT_PROGBITS)
#define TEXTSEC(s)		(s->sh_type == SHT_PROGBITS && s->sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
#define DATASEC(s)		(s->sh_type == SHT_PROGBITS && (s->sh_flags & SHF_ALLOC))
#define BSSSEC(s)		(s->sh_type == SHT_NOBITS && s->sh_flags == (SHF_ALLOC | SHF_WRITE))
#define DEBUGSEC(n)		(!ft_strncmp(n, ".debug_", 7))

#define SEC_CODE			0x010
#define SEC_DATA			0x020
#define SEC_READONLY		0x008
#define SEC_SMALL_DATA		0x400000
#define SEC_HAS_CONTENTS	0x100
#define SEC_DEBUGGING		0x2000

#define MINTYPE	"abcdgrst"

int		parse_opt(data_t *data, char **argv, int argc);
int		parse_file(char *file, data_t *data);
int		parse_header(fdata_t *fdata);
int		parse_shdrtab_32(data_t *data, fdata_t *fdata);
int		parse_shdrtab_64(data_t *data, fdata_t *fdata);
int		parse_symtab_64(data_t *data, fdata_t *fdata, Elf64_Shdr *shsymtab);
int		parse_symtab_32(data_t *data, fdata_t *fdata, Elf32_Shdr *shsymtab);
int		check_shdr(void *sh, fdata_t *fdata);

int		add_symbol(fdata_t *fdata, symbol_t sym);
void	remove_symbol(fdata_t *fdata, symlist_t *elem);
void	print_symbols(data_t *data, int c);
void	sort_symlist(fdata_t *fdata, int cmp);
void	reverse_symlist(fdata_t *fdata);
void	free_symlist(symlist_t *symlist);

#endif // FT_NM
