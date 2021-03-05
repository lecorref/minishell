/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 10:14:35 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/18 23:48:42 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdup(char const *s1)
{
	char	*dest;

	dest = (char*)malloc(sizeof(char) * ft_strlen(s1) + 1);
	if (dest == NULL)
		return (NULL);
	ft_strcpy(dest, s1);
	return (dest);
}
