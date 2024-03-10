/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsabbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 16:03:08 by rsabbah           #+#    #+#             */
/*   Updated: 2022/12/13 16:14:06 by rsabbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_base_fd(long nbr, const char *base, int fd)
{
	size_t	blen;
	char	c;

	blen = ft_strlen(base);
	if (nbr < 0)
		nbr *= -1;
	c = base[nbr % blen];
	if ((size_t)nbr >= blen)
		return (ft_putnbr_base_fd(nbr / blen, base, fd) + write(1, &c, 1));
	return (write(fd, &base[nbr % blen], 1));
}
