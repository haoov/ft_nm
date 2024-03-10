#include "ft_nm.h"

int parse_opt(data_t *data, char **argv) {
	int i;
	char *f;
	for (i = 0; (argv[i] && argv[i][0] == '-'); i++) {
		for (int j = 1; argv[i][j]; j++) {
			if ((f = ft_strchr(OPTS, argv[i][j]))) {
				data->opt |= (1 << (f - OPTS));
				if (argv[i][j] == 'p')
					data->opt &= ~ONUM;
			}
			else
				error(ERIOPT, &argv[i][j]);
		}
	}
	return (i);
}
