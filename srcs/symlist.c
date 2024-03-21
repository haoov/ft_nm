#include "ft_nm.h"

char *ign_char = "._";

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
	if (!symlist)
		ft_printf(1, "ft_nm: %s: no symbols\n", data->fdata.name);
	while (symlist) {
		if (ft_strchr("Uwv", symlist->sym.type))
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

int compare(symbol_t s1, symbol_t s2) {
	char c1 = 0, c2 = 0;

	if (s1.name && s2.name) {
		while (*s1.name && *s2.name) {
			while (ft_strchr(ign_char, *s1.name))
				s1.name++;
			while (ft_strchr(ign_char, *s2.name))
				s2.name++;
			if (*s1.name >= 'A' && *s1.name <= 'Z')
				c1 = *s1.name + 32;
			else
				c1 = *s1.name;
			if (*s2.name >= 'A' && *s2.name <= 'Z')
				c2 = *s2.name + 32;
			else
				c2 = *s2.name;
			if (c1 != c2)
				break;
			s1.name++;
			s2.name++;
		}
	}
	if (!(c1 - c2)) {
		if (s1.value != s2.value)
			return (s1.value - s2.value);
		else
			return (s1.type - s2.type);
	}
	return (c1 - c2);
}

void sort_symlist(fdata_t *fdata, int cmp) {
    symlist_t *start = fdata->symlist;
    symlist_t *tmp;
    int swapped;

    do {
        swapped = 0;
        tmp = start;

        while (tmp && tmp->next != NULL) {
            if (cmp == ULCMP) {
                if (tmp->sym.value > tmp->next->sym.value) {
                    swap_symbols(tmp, tmp->next);
                    swapped = 1;
                }
				else if (tmp->sym.value == tmp->next->sym.value) {
					if (tmp->sym.type == 'U' && tmp->next->sym.type != 'U') {
						swap_symbols(tmp, tmp->next);
						swapped = 1;
					}
					else if (ft_strcmp(tmp->sym.name, tmp->next->sym.name) > 0) {
						swap_symbols(tmp, tmp->next);
						swapped = 1;
					}
				}
            }
			else {
                if (compare(tmp->sym, tmp->next->sym) > 0) {
                    swap_symbols(tmp, tmp->next);
                    swapped = 1;
                }
            }
            tmp = tmp->next;
        }
    } while (swapped);
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
