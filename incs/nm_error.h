#ifndef NM_ERROR_H
#define NM_ERROR_H

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "ft_nm_types.h"

#define ERIOPT		1
#define ERNOFILE	2
#define ERWFFMT		3
#define ERRNO		4

#define	ERIOPT_MSG		"invalid option --"
#define	ERNOFILE_MSG	"no such file"
#define	ERWFFMT_MSG		"file format not recognized"

typedef struct error_s {
	uint8_t e;
	char *i;
} error_t;

error_t *error_location(void);

#define error (*error_location())

#define SET_ERROR(er, if) (error.e = er, error.i = if)

void print_error(void);

#endif // NM_ERROR_H