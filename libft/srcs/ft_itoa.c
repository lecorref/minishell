/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:36:04 by jfreitas          #+#    #+#             */
/*   Updated: 2019/08/16 11:30:30 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char			*ft_itoa(int n)
{
	size_t	len;
	size_t	neg;
	char	*fresh_str;

	len = ft_intlen(n);
	if (!(fresh_str = (char*)malloc(sizeof(char) * len + 1)))
		return (NULL);
	fresh_str[len] = '\0';
	if (n < 0)
	{
		fresh_str[0] = '-';
		neg = 1;
	}
	else
		neg = 0;
	while (len > neg)
	{
		len--;
		fresh_str[len] = 48 + n % 10 * (n < 0 ? -1 : 1);
		n = n / 10;
	}
	return (fresh_str);
}
