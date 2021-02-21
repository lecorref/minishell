/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_newstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 11:23:47 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/19 00:11:08 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int		join_newstr(char **str, const char *src)
{
	char	*tmp;
	int		len;

	len = ft_strlen(*str) + ft_strlen(src);
	if (!(tmp = ft_strnew(len)))
		return (0);
	len = -1;
	while ((*str)[++len])
		tmp[len] = (*str)[len];
	while (*src)
		tmp[len++] = *src++;
	free(*str);
	*str = tmp;
	return (1);
}
