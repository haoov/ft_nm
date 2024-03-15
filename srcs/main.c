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
		else
			print_symbols(&data);
		i++;
	}
	return (0);
}

void print_symbols(data_t *data) {
	symlist_t *symlist = data->fdata.symlist;
	while (symlist) {
		printf("%.16x %c %s\n", (uint16_t)symlist->sym.value, symlist->sym.type, symlist->sym.name);
		symlist = symlist->next;
	}
}
