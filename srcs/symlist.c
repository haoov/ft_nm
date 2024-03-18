#include "ft_nm.h"

int add_symbol(fdata_t *fdata, symbol_t sym) {
	symlist_t *new;
	symlist_t *last;

	if (!(new = (symlist_t*)malloc(sizeof(symlist_t)))) {
		SET_ERROR(ERMEM);
		return (-1);
	}
	new->sym = sym;
	new->next = NULL;
	last = fdata->symlist;
	while (last && last->next)
		last = last->next;
	if (!last)
		fdata->symlist = new;
	else
		last->next = new;
}

void remove_symbol(fdata_t *fdata, symlist_t *elem) {
	symlist_t *tmp;

	if (fdata->symlist == elem) {
		fdata->symlist = elem->next;
		free(elem);
	}
	else {
		tmp = fdata->symlist;
		while (tmp->next != elem)
			tmp = tmp->next;
		tmp->next = elem->next;
		free(elem);
	}
}

void print_symbols(data_t *data, int c) {
	if (c > 1)
		ft_printf(1, "\n%s:\n", data->fdata.name);
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

void free_symlist(symlist_t *symlist) {
	symlist_t *tmp;

	while (symlist) {
		tmp = symlist;
		symlist = symlist->next;
		free(tmp);
	}
}

void swap_symbols(symlist_t *a, symlist_t *b) {
	symbol_t tmp;

	tmp = a->sym;
	a->sym = b->sym;
	b->sym = tmp;
}

void sort_symlist(fdata_t *fdata, int cmp) {
	symlist_t *symlist = fdata->symlist;
	symlist_t *tmp;

	while (symlist) {
		tmp = symlist;
		while (tmp->next) {
			if (cmp == ULCMP) {
				if (tmp->sym.value > tmp->next->sym.value)
					swap_symbols(tmp, tmp->next);
			}
			else {
				if (ft_strcmp(tmp->sym.name, tmp->next->sym.name) > 0)
					swap_symbols(tmp, tmp->next);
			}
			tmp = tmp->next;
		}
		symlist = symlist->next;
	}
}

void reverse_symlist(fdata_t *fdata) {
	symlist_t *symlist = fdata->symlist;
	symlist_t *prev = NULL;
	symlist_t *next;

	while (symlist) {
		next = symlist->next;
		symlist->next = prev;
		prev = symlist;
		symlist = next;
	}
	fdata->symlist = prev;
}