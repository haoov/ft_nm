#ifndef NM_ERROR_H
#define NM_ERROR_H

#include <stdint.h>

#define ERIOPT		1
#define ERNOFILE	2
#define ERWFFMT		3

#define	ERIOPT_MSG		"invalid option --"
#define	ERNOFILE_MSG	"no such file"
#define	ERWFFMT_MSG		"file format not recognized"

void error(uint8_t e, char *data);

#endif // NM_ERROR_H