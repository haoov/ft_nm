#include "ft_nm.h"

int count_files(int argc, char **argv, int i) {
	int c = 0;

	while (i < argc) {
		if (argv[i][0] != '-')
			c++;
		i++;
	}
	return (c);
}

void clean_up(fdata_t *fdata) {
	if (fdata->fd > 0)
		close(fdata->fd);
	if (fdata->map)
		munmap(fdata->map, fdata->st.st_size);
	if (fdata->symlist) {
		free_symlist(fdata->symlist);
		fdata->symlist = NULL;
	}
}

void apply_opt(data_t *data) {
	symlist_t *symlist = data->fdata.symlist;
	symlist_t *tmp;

	while (symlist) {
		tmp = symlist->next;
		if (data->opt & OUND && symlist->sym.type != 'U')
			remove_symbol(&data->fdata, symlist);
		else if (data->opt & OGLOB && ELF_ST_BIND(symlist->sym.info) != STB_GLOBAL)
			remove_symbol(&data->fdata, symlist);
		else if (!(data->opt & OALL) && ELF_ST_BIND(symlist->sym.info) == STB_LOCAL)
			remove_symbol(&data->fdata, symlist);
		symlist = tmp;
	}
	if (data->opt & ONSRT)
		return;
	else if (data->opt & ONUM)
		sort_symlist(&data->fdata, ULCMP);
	else
		sort_symlist(&data->fdata, STRCMP);
	if (data->opt & OREV)
		reverse_symlist(&data->fdata);
}

int main(int argc, char **argv) {
	int i = 1, c = 0;
	data_t data = {
		.opt = 0,
		.fdata = {
			.fd = 0,
			.name = NULL,
			.st = {0},
			.map = NULL
			}
		};
	parse_opt(&data, argv, argc);
	while (i < argc && argv[i][0] == '-')
		i++;
	c = count_files(argc, argv, i);
	while (i < argc && argv[i][0] != '-') {
		if (parse_file(argv[i], &data) == -1)
			print_error(error, argv[i]);
		else {
			apply_opt(&data);
			print_symbols(&data, c);
		}
		clean_up(&data.fdata);
		i++;
	}
	return (0);
}
