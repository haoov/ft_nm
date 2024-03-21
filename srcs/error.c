#include "ft_nm.h"

uint8_t error_g = 0;

char *err_msgs[] = {
	ERIOPT_MSG, ERNOFILE_MSG, ERWFFMT_MSG, EREMPTY_MSG, ERRCORRUPT_MSG,
	ERMEM_MSG, ERISDIR_MSG 
};

uint8_t *error_location(void) {
	return (&error_g);
}

void print_error(uint8_t err, char *i) {
	if (err == ERRNO)
		ft_printf(2, "ft_nm: %s: %s\n", i, strerror(errno));
	else if (err == ERIOPT)
		ft_printf(2, "ft_nm: %s \'%c\'\n", err_msgs[err - 1], *i);
	else if (err == ERNOFILE)
		ft_printf(2, "ft_nm: \'%s\': %s\n", i, err_msgs[err - 1]);
	else
		ft_printf(2, "ft_nm: %s: %s\n", i, err_msgs[err - 1]);
}
