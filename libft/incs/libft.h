#ifndef LIBFT_H
#define LIBFT_H

#include <stdio.h>

int		ft_printf(int fd, const char *format, ...);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *s, int c, size_t n);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif // LIBFT_H