#include "libft.h"

size_t	ft_strnlen(const char *s, size_t n)
{
	size_t	len;

	len = 0;
	while (s[len] && n-- > 0)
		len++;
	return (len);
}