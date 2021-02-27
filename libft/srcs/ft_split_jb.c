/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_jb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 18:04:52 by jle-corr          #+#    #+#             */
/*   Updated: 2021/02/27 01:23:11 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			split_counter(char const *s, char c)
{
	char	*spliter;
	int		i;

	i = 0;
	while (s && *s)
	{
		if (((spliter = ft_strchr(s, c)) - s) != 0)
			i++;
		s = (spliter ? spliter + 1 : NULL);
	}
	return (i + 1);
}

char		**ft_split_jb(char const *s, char c)
{
	char	**tab;
	char	*adr;
	int		i;

	if (!(tab = (char**)malloc(sizeof(*tab) * split_counter(s, c))))
		return (NULL);
	i = 0;
	while (s && *s)
	{
		if (((adr = ft_strchr(s, c)) - s) != 0)
			if (!(tab[i++] = ft_substr(s, 0, (adr ? adr - s : ft_strlen(s)))))
				return (ft_freetab(tab));
		s = (adr ? adr + 1 : NULL);
	}
	tab[i] = NULL;
	return (tab);
}
