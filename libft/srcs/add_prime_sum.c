/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_prime_sum.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 11:06:17 by jfreitas          #+#    #+#             */
/*   Updated: 2019/08/14 16:52:09 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int		add_prime_sum(unsigned int n)
{
	unsigned int	i;
	unsigned int	res;

	res = 0;
	i = 2;
	if (n <= 1)
		return (0);
	while (i <= n)
	{
		if (is_prime(i) == 1)
			res = res + i;
		i++;
	}
	return (res);
}
