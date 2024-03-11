#include "ft_nm.h"

void exit_prgm(int status, data_t data) {
	if (data.fdata.fd != -1)
		close(data.fdata.fd);
	if (status)
		print_error();
	exit(status);
}

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
	if (i == -1)
		exit_prgm(1, data);
	if (argc == 1) {
		if (parse_file("a.out", &data) == -1)
			exit_prgm(1, data);
	}
	for (; argv[i]; i++) {
		if (parse_file(argv[i], &data) == -1)
			print_error();
	}
	exit_prgm(0, data);
}


	// for (int i = 0; i < ehdr->e_shnum; i++) {
	// 	shdr = (Elf64_Shdr *)((char *)fdata->map + ehdr->e_shoff + i * ehdr->e_shentsize);
	// 	if (shdr->sh_type == SHT_STRTAB) {
	// 		char *strtab = (char *)fdata->map + shdr->sh_offset;
	// 		for (int j = 0; j < shdr->sh_size; j += ft_strlen(strtab + j) + 1) {
	// 			ft_printf(1, "Section name: %s\n", strtab + j);
	// 		}
	// 	}
	// }