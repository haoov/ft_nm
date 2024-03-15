#include "ft_nm.h"

int main(int argc, char **argv) {
	int i;
	data_t data = {
		.opt = 0,
		.fdata = {
			.fd = 0,
			.name = NULL,
			.st = {0},
			.map = NULL
			}
		};
	i = parse_opt(&data, argv);
	while (i < argc) {
		if (parse_file(argv[i], &data) == -1)
			print_error(error, argv[i]);
		else {
			apply_opt(&data);
			print_symbols(&data);
		}
		i++;
	}
	return (0);
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
}
