/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 11:05:51 by jfreitas          #+#    #+#             */
/*   Updated: 2021/01/27 00:03:33 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_putstr_list(t_list *str)
{
	while (((char*)str->content))
	{
		write(1, &str->content, 1);
		str->content++;
	}
	str = str->next;
}
