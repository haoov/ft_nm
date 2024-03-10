/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_addr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsabbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 16:15:09 by rsabbah           #+#    #+#             */
/*   Updated: 2022/12/13 16:16:09 by rsabbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	put_addr(int fd, unsigned long addr)
{
	static char	*base = HEX_BASE;
	char		c;

	c = base[addr % 16];
	if (addr >= 16)
		return (put_addr(fd, addr / 16) + write(fd, &c, 1));
	return (write(1, &c, 1));
}

int	print_addr(t_conf conf)
{
	unsigned long	addr;
	int				count;

	count = 0;
	addr = (unsigned long)conf.value.t_ptr;
	if (conf.flag.zero == FALSE)
		count += print_infos((long)addr, conf);
	while (conf.p_value > 16)
	{
		count += write(conf.fd, "0", 1);
		conf.p_value--;
	}
	count += put_addr(conf.fd, addr);
	return (count);
}
