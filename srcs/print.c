#include "ft_nm.h"

void print_symbols(data_t *data) {
	symlist_t *symlist = data->fdata.symlist;
	while (symlist) {
		if (symlist->sym.type == 'U')
			ft_printf(1, "%16s", "");
		else
			ft_printf(1, "%.16x", (uint32_t)symlist->sym.value);
		ft_printf(1, " %c ", symlist->sym.type);
		ft_printf(1, "%s\n", symlist->sym.name);
		symlist = symlist->next;
	}
}