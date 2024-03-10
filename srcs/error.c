#include "ft_nm.h"

char *err_msgs[] = {
	ERIOPT_MSG, ERNOFILE_MSG, ERWFFMT_MSG
};

void error(uint8_t e, char *data) {
	if (e == ERIOPT)
		ft_printf(2, "ft_nm: %s \'%c\'\n", err_msgs[e - 1], *data);
	else
		ft_printf(2, "ft_nm: %s: %s\n", data, err_msgs[e - 1]);
	exit (1);
}