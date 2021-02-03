/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 13:30:29 by jfreitas          #+#    #+#             */
/*   Updated: 2019/07/21 14:46:00 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*fresh_sub_str;

	fresh_sub_str = ft_strnew(len);
	if (fresh_sub_str == NULL || s == NULL)
		return (NULL);
	ft_strncpy(fresh_sub_str, s + start, len);
	return (fresh_sub_str);
}
/*
or

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*fresh_sub_str;

	i = 0;
	if (!s)
		return (NULL);
	if (!(fresh_sub_str = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < (len + 1))
		fresh_sub_str[i++] = '\0';
	i = 0;
	while (i < len)
		fresh_sub_str[i++] = s[start++];
	fresh_sub_str[i] = '\0';
	return (fresh_sub_str);
}*/
