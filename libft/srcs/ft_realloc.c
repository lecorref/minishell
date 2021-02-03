/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 11:23:47 by jfreitas          #+#    #+#             */
/*   Updated: 2019/08/14 13:51:31 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr)
	{
		if (size)
		{
			if (!(new_ptr = ft_memalloc(size)))
				return (NULL);
			ft_bzero(new_ptr, size);
			ft_memcpy(new_ptr, ptr, size);
		}
		else
		{
			if (!(new_ptr = (unsigned char*)malloc(sizeof(ptr))))
				return (NULL);
		}
		free(ptr);
		return (new_ptr);
	}
	return ((unsigned char*)malloc(sizeof(ptr) * size));
}
