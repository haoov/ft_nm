#include "ft_nm.h"

int parse_symtab_32(fdata_t *fdata, Elf32_Shdr *shsymtab) {
	char		*strtab;
	Elf32_Sym	*sym;
	Elf32_Shdr	*sh;
	size_t		idx, j, pos, sh_off, sym_off;

	for (int i = 0; i < (shsymtab->sh_size / shsymtab->sh_entsize); i++) {
		//get current symbol
		sym_off = shsymtab->sh_offset + i * shsymtab->sh_entsize;
		sym = (Elf32_Sym*)((byte_t*)fdata->map + sym_off);

		//if the symbol is a section
		if ((sym->st_info & 0x0F) == STT_SECTION) {
			//check section index
			if (sym->st_shndx > fdata->shtab_count) {
				SET_ERROR(ERWFFMT);
				return (-1);
			}

			//get related section header
			sh_off = fdata->shtab_off + sym->st_shndx * fdata->shtab_entsize;
			sh = (Elf32_Shdr*)((byte_t*)fdata->map + sh_off);

			//check section header
			if (check_shdr(sh, fdata) == -1)
				return (-1);

			//get related string table
			strtab = (char*)((byte_t*)fdata->map + fdata->shstrtab_off);

			//get string table name index
			idx = sh->sh_name;
		}

		else {
			//get related string table section header
			sh_off = fdata->shtab_off + shsymtab->sh_link * fdata->shtab_entsize;
			sh = (Elf32_Shdr*)((byte_t*)fdata->map + sh_off);

			//check section header
			if (check_shdr(sh, fdata) == -1)
				return (-1);

			//get related string table
			strtab = (char*)((byte_t*)fdata->map + sh->sh_offset);

			//get string table name index
			idx = sym->st_name;
		}

		j = 0;
		pos = 0;
		while (j < idx && pos < sh->sh_size) {
			pos += ft_strnlen(strtab + pos, fdata->st.st_size - pos);
			j++;
		}

		ft_printf(1, "%.16x %c %s\n", sym->st_value, 'T', strtab + idx + pos);
	}

	return (0);
}

int parse_symtab_64(fdata_t *fdata, Elf64_Shdr *shsymtab) {
	char		*strtab;
	Elf64_Sym	*sym;
	Elf64_Shdr	*sh;
	size_t		idx, j, pos, sh_off, sym_off;

	for (int i = 0; i < (shsymtab->sh_size / shsymtab->sh_entsize); i++) {
		//get current symbol
		sym_off = shsymtab->sh_offset + i * shsymtab->sh_entsize;
		sym = (Elf64_Sym*)((byte_t*)fdata->map + sym_off);

		//if the symbol is a section
		if ((sym->st_info & 0x0F) == STT_SECTION) {
			//check section index
			if (sym->st_shndx > fdata->shtab_count) {
				SET_ERROR(ERWFFMT);
				return (-1);
			}

			//get related section header
			sh_off = fdata->shtab_off + sym->st_shndx * fdata->shtab_entsize;
			sh = (Elf64_Shdr*)((byte_t*)fdata->map + sh_off);

			//check section header
			if (check_shdr(sh, fdata) == -1)
				return (-1);

			//get related string table
			strtab = (char*)((byte_t*)fdata->map + fdata->shstrtab_off);

			//get string table name index
			idx = sh->sh_name;
		}

		else {
			//get related string table section header
			sh_off = fdata->shtab_off + shsymtab->sh_link * fdata->shtab_entsize;
			sh = (Elf64_Shdr*)((byte_t*)fdata->map + sh_off);

			//check section header
			if (check_shdr(sh, fdata) == -1)
				return (-1);

			//get related string table
			strtab = (char*)((byte_t*)fdata->map + sh->sh_offset);

			//get string table name index
			idx = sym->st_name;
		}

		//get string table name
		j = 0;
		pos = 0;
		while (j < idx && pos < sh->sh_size) {
			pos += ft_strnlen(strtab + pos, fdata->st.st_size - pos);
			j++;
		}

		ft_printf(1, "%.16x %c %s\n", sym->st_value, 'T', strtab + idx + pos);

		symbol_t symdata = {
			.value = sym->st_value,
			.info = sym->st_info,
			.name = strtab + idx + pos
		};
		add_symbol(fdata, symdata);
	}

	return (0);
}

int add_symbol(fdata_t *fdata, symbol_t sym) {
	symlist_t *new;
	symlist_t *last;

	if (!(new = (symlist_t*)malloc(sizeof(symlist_t)))) {
		SET_ERROR(ERMEM);
		return (-1);
	}
	*(new->sym) = sym;
	new->next = NULL;
	last = fdata->symlist;
	while (last && last->next)
		last = last->next;
	if (!last)
		fdata->symlist = new;
	else
		last->next = new;
}