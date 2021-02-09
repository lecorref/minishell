/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_newstr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 16:13:53 by jle-corr          #+#    #+#             */
/*   Updated: 2021/02/09 18:19:59 by jle-corr         ###   ########.fr       */
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
