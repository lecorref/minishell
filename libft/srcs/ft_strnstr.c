/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 15:11:35 by jfreitas          #+#    #+#             */
/*   Updated: 2019/07/15 12:23:23 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strnstr(char const *haystack, char const *needle, size_t len)
{
	size_t	j;
	char	*str;
	char	*to_find;
	int		length;

	j = 0;
	str = (char*)haystack;
	to_find = (char*)needle;
	length = ft_strlen(to_find);
	if (!*to_find)
		return (str);
	while (str[j] && (j + length) <= len)
	{
		if (ft_strncmp(haystack + j, needle, length) == 0)
			return (str + j);
		j++;
	}
	return (NULL);
}
