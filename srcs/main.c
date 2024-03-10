#include "ft_nm.h"

int main(int argc, char **argv) {
	(void)argc;
	int i = 1;
	data_t data = {0};
	i += parse_opt(&data, argv + 1);
}
