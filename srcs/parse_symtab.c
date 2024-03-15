#include "ft_nm.h"

char *get_name(char *strtab, size_t idx, size_t shsize) {
	size_t j = 0, pos = 0;

	while (j < idx && pos < shsize) {
		pos += ft_strnlen(strtab + pos, shsize - pos);
		j++;
	}
	if ((j < idx && pos == shsize) || pos + idx > shsize) {
		SET_ERROR(ERWFFMT);
		return (NULL);
	}
	return (strtab + idx + pos);
}

char *get_shdr_name(fdata_t *fdata, size_t idx) {
	Elf64_Shdr *sh_strtab = (Elf64_Shdr*)SHDR(fdata, fdata->shtab.sh_stidx);
	return (get_name(STRTAB(fdata, sh_strtab), idx, sh_strtab->sh_size));
}

char *symbol_name_64(Elf64_Sym *sym, Elf64_Shdr *sh_symtab, Elf64_Shdr *sh_strtab, fdata_t *fdata) {
	Elf64_Shdr	*sh;

	//if the symbol is a section
	if (ELF_ST_TYPE(sym->st_info) == STT_SECTION) {
		//check section index
		if (sym->st_shndx > fdata->shtab.count) {
			SET_ERROR(ERWFFMT);
			return (NULL);
		}

		//get related section header
		sh = (Elf64_Shdr*)SHDR(fdata, sym->st_shndx);

		//check section header
		if (check_shdr(sh, fdata) == -1)
			return (NULL);

		//get related string table section header
		sh_strtab = (Elf64_Shdr*)SHDR(fdata, fdata->shtab.sh_stidx);

		//check section header
		if (check_shdr(sh_strtab, fdata) == -1)
			return (NULL);

		//get string table
		return (get_name(STRTAB(fdata, sh_strtab), sh->sh_name, sh_strtab->sh_size));
	}
	else {
		//get related string table section header
		sh_strtab = (Elf64_Shdr*)SHDR(fdata, sh_symtab->sh_link);

		//check section header
		if (check_shdr(sh_strtab, fdata) == -1)
			return (NULL);

		//get string table
		return (get_name(STRTAB(fdata, sh_strtab), sym->st_name, sh_strtab->sh_size));
	}
}

uint8_t symbol_type_64(Elf64_Sym *sym, Elf64_Shdr *sh_strtab, fdata_t *fdata) {
	uint8_t 	type;
	char		*sh_name;

	//if section indice is reserved
	if (sym->st_shndx >= SHN_LORESERVE && sym->st_shndx <= SHN_HIRESERVE) {
		if (sym->st_shndx == SHN_ABS)
			//absolute symbol
			type = 'a';
		else if (sym->st_shndx == SHN_COMMON)
			//common symbol
			type = 'c';
		else if (sym->st_shndx == SHN_UNDEF)
			//undefined symbol
			type = 'U';
		if (ELF_ST_BIND(sym->st_info) == STT_GNU_IFUNC)
			type = 'i';
		else if (ELF_ST_BIND(sym->st_info) == STB_WEAK) {
			if (ELF_ST_TYPE(sym->st_info) == STT_OBJECT)
				type = 'v';
			else
				type = 'w';
		}
	}
	else {
		//get related section header
		Elf64_Shdr *sh = (Elf64_Shdr*)SHDR(fdata, sym->st_shndx);

		//check section header
		if (check_shdr(sh, fdata) == -1)
			return (0);

		//get section name
		sh_name = get_name(STRTAB(fdata, sh_strtab), sh->sh_name, sh_strtab->sh_size);

		ft_printf(1, "sh_name: %s\n", sh_name);

		//check section name
		if (sh_name == NULL)
			return (0);

		//get section type
		if (TEXTSEC(sh) && !ft_strcmp(sh_name, ".text"))
			type = 't';
		else if (DATASEC(sh) && !ft_strcmp(sh_name, ".data"))
			type = 'd';
		else if (BSSSEC(sh) && !ft_strcmp(sh_name, ".bss"))
			type = 'b';
		else if (BSSSEC(sh) && !ft_strcmp(sh_name, ".sbss"))
			type = 's';
		else if (DATASEC(sh) && !ft_strcmp(sh_name, ".rodata"))
			type = 'r';
		else if (DATASEC(sh) && !ft_strcmp(sh_name, ".sdata"))
			type = 'g';
		else if (DEBUGSEC(sh_name))
			type = 'N';
		else if (EHFRAMESEC(sh_name))
			type = 'p';

		if (ELF_ST_BIND(sym->st_info) == STT_GNU_IFUNC)
			type = 'i';
		else if (ELF_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
			type = 'u';
		else if (ELF_ST_BIND(sym->st_info) == STB_WEAK) {
			if (ELF_ST_TYPE(sym->st_info) == STT_OBJECT)
				type = 'V';
			else
				type = 'W';
		}
	}
	if (type == 0)
		type = '?';
	if (ft_strchr(MINTYPE, type) && ELF_ST_BIND(sym->st_info) == STB_GLOBAL)
		type = ft_toupper(type);

	return (type);
}

int parse_symtab_64(fdata_t *fdata, Elf64_Shdr *sh_symtab) {
	Elf64_Sym	*sym;
	size_t		sym_off;
	symbol_t	symbol;
	Elf64_Shdr	*sh_strtab;

	//get section header string table
	sh_strtab = (Elf64_Shdr*)SHDR(fdata, fdata->shtab.sh_stidx);

	//check section header
	if (check_shdr(sh_strtab, fdata) == -1)
		return (-1);

	for (int i = 0; i < (sh_symtab->sh_size / sh_symtab->sh_entsize); i++) {
		//get current symbol
		sym_off = sh_symtab->sh_offset + i * sh_symtab->sh_entsize;
		sym = (Elf64_Sym*)((byte_t*)fdata->map + sym_off);

		//get symbol name
		symbol.name = symbol_name_64(sym, sh_symtab, sh_strtab, fdata);
		if (symbol.name == NULL)
			return (-1);

		//get symbol type
		symbol.type = symbol_type_64(sym, sh_strtab, fdata);

		symbol.value = sym->st_value;

		if (ELF_ST_TYPE(sym->st_info) == STT_NOTYPE && ELF_ST_BIND(sym->st_info) == STB_LOCAL)
			continue;
		add_symbol(fdata, symbol);
	}

	return (0);
}
