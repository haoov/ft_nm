#include "ft_nm.h"

int parse_opt(data_t *data, char **argv, int argc) {
	int		i = 1;
	char	*f;
	
	while (i < argc) {
		if (argv[i] && (argv[i][0] != '-' || ft_strlen(argv[i]) == 1)) {
			i++;
			continue;
		}
		for (int j = 1; argv[i][j]; j++) {
			if ((f = ft_strchr(OPTS, argv[i][j]))) {
				data->opt |= (1 << (f - OPTS));
				if (argv[i][j] == 'p') {
					data->opt &= ~ONUM;
					data->opt &= ~OREV;
				}
				if (argv[i][j] == 'n')
					data->opt &= ~ONSRT;
				if (argv[i][j] == 'u') {
					data->opt &= ~OALL;
					data->opt &= ~OGLOB;
				}
				if (argv[i][j] == 'g')
					data->opt &= ~OALL;
			}
			else {
				print_error(ERIOPT, &argv[i][j]);
				exit(-1);
			}
		}
		i++;
	}
	return (i);
}
