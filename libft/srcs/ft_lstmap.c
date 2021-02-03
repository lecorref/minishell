/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 15:28:16 by jfreitas          #+#    #+#             */
/*   Updated: 2019/08/29 12:14:05 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *newlst;

	if (lst)
	{
		newlst = (t_list*)malloc(sizeof(lst));
		if (newlst == NULL)
			return (NULL);
		newlst = (*f)(lst);
		newlst->next = ft_lstmap(lst->next, (*f));
		return (newlst);
	}
	return (NULL);
}
