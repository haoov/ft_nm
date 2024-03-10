#include "ft_nm.h"

error_t error_g = {0, 0};

char *err_msgs[] = {
	ERIOPT_MSG, ERNOFILE_MSG, ERWFFMT_MSG
};

error_t *error_location(void) {
	return (&error_g);
}

void print_error(void) {
	if (error.e == ERRNO) {
		if (errno == EISDIR)
			ft_printf(2, "ft_nm: Warning: \'%s\' is a directory\n", error.i);
		else
			ft_printf(2, "ft_nm: %s: %s\n", error.i, strerror(errno));
	}
	else if (error.e == ERIOPT)
		ft_printf(2, "ft_nm: %s \'%c\'\n", err_msgs[error.e - 1], *error.i);
	else if (error.e == ERNOFILE)
		ft_printf(2, "ft_nm: \'%s\': %s\n", error.i, err_msgs[error.e - 1]);
	else
		ft_printf(2, "ft_nm: %s: %s\n", error.i, err_msgs[error.e - 1]);
}