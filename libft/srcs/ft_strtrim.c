/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:35:13 by jfreitas          #+#    #+#             */
/*   Updated: 2019/07/22 19:15:58 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	i;
	char	*str;

	if (s)
	{
		str = ft_strdup(s);
		if (str == NULL || s == NULL)
			return (NULL);
		while (*str == ' ' || *str == '\n' || *str == '\t')
			str++;
		i = ft_strlen(str) - 1;
		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			i--;
		str[++i] = '\0';
		return (ft_strdup(str));
	}
	return (0);
}
