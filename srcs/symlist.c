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

void free_symlist(symlist_t *symlist) {
	symlist_t *tmp;

	while (symlist) {
		tmp = symlist;
		symlist = symlist->next;
		free(tmp);
	}
}