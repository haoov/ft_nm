#include "nm_error.h"

char *err_msgs[] = {
	ERIOPT_MSG, ERNOFILE_MSG, ERWFFMT_MSG
};

void error(uint8_t e, char *data) {
	ft_putstr_fd("ft_nm: ", 2);
	if (e == ERIOPT) {
		ft_putstr_fd(err_msgs[0], 2);
		ft_putstr_fd(' \'', 2);
		ft_putchar_fd(*data, 2);
		ft_putchar_fd('\'', 2);
	}
	else {
		ft_putstr_fd(data, 2);
		ft_putstr_fd(': ', 2);
		ft_putstr_fd(err_msgs[e], 2);
	}
	ft_putchar_fd('\n', 2);
	exit (1);
}