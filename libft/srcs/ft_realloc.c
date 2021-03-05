/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 14:48:23 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/19 14:48:38 by jfreitas         ###   ########.fr       */
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
