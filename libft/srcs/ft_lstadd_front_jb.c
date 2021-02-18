#include "libft.h"

void	ft_lstadd_front_jb(t_listjb **alst, t_listjb *new)
{
	if (!new)
		return ;
	new->next = *alst;
	*alst = new;
}
