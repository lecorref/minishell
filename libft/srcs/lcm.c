/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/26 16:57:33 by exam              #+#    #+#             */
/*   Updated: 2019/08/14 16:56:53 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

unsigned int	lcm(unsigned int a, unsigned int b)
{
	int	i;

	if (a > b)
		i = a;
	if (a < b)
		i = b;
	if (a == 0 || b == 0)
		return (0);
	while (i)
	{
		if (i % a == 0 && i % b == 0)
			return (i);
		i++;
	}
	return (0);
}
