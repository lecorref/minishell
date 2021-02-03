/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 12:19:28 by jfreitas          #+#    #+#             */
/*   Updated: 2021/01/26 23:59:38 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

/*
** OBS.: EOF is an integer, not a character, and if you want to check for it,
** you need to use an int
*/

static int	check_line(char *str)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = str;
	while (tmp[i] != '\n')
		if (!(tmp[i++]))
			return (0);
	return (1);
}

static char	*read_file(int fd, char **str, char **line)
{
	char	buff[BUFFER_SIZE + 1];
	int		ret;
	char	*tmp;

	if (fd < 0 || !line || BUFFER_SIZE <= 0 || read(fd, buff, 0) == -1)
		return (NULL);
	if (!*str)
	{
		if (!(*str = ft_strnew(BUFFER_SIZE + 1)))
			return (NULL);
	}
	while ((check_line(*str) == 0) && (ret = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[ret] = '\0';
		tmp = *str;
		if (!(*str = ft_strjoin(tmp, buff)))
			return (NULL);
		free(tmp);
	}
	return (*str);
}

int			get_next_line(int fd, char **line)
{
	static char	*str;

	*line = NULL;
	if (read_file(fd, &str, line) == NULL)
		return (-1);
	else
		str = read_file(fd, &str, line);
	if (!*str)
		*line = ft_strdup("");
	else
		*line = ft_substr(str, 0, ft_strclen(str, '\n'));
	if (check_line(str) == 1)
	{
		ft_memmove(str, ft_strchr(str, '\n') + 1, ft_strclen(str, '\0'));
		return (1);
	}
	free(str);
	str = NULL;
	return (0);
}
