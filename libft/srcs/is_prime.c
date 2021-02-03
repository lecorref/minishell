/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_prime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 12:44:52 by jfreitas          #+#    #+#             */
/*   Updated: 2019/08/14 13:42:55 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int		is_prime(unsigned int n)
{
	unsigned int	i;

	i = 2;
	if (n <= 1)
		return (0);
	while (i <= n / 2)
	{
		if (n % i == 0)
			return (0);
		else
			i++;
	}
	return (1);
}
