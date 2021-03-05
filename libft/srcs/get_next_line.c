/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:06:18 by jle-corr          #+#    #+#             */
/*   Updated: 2020/02/21 01:13:48 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		get_next_line(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
			return (0);
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}
