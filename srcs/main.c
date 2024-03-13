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
	if (argc == 1) {
		if (parse_file("a.out", &data) == -1)
			return(1);
	}
	while (i < argc) {
		if (parse_file(argv[i], &data) == -1)
			print_error(error, argv[i]);
		i++;
	}
	return (0);
}
