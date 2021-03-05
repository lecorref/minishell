/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 14:04:05 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/27 18:29:26 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

/*
void	ft_putstr_fd(char *s, int fd)
{
	if (s)
	{
		while (*s != '\0')
		{
			write(fd, s, 1);
			s++;
		}
	}
}
*/

//PROPOSITION : code below should be better bc you do only one system call,
//instead of as much system call as there is characters in string (which is a
//lack of performance);

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return;
	write(fd, s, ft_strlen(s));
}
