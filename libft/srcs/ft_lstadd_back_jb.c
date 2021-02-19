#include "libft.h"

void		ft_lstadd_back_jb(t_listjb **alst, t_listjb *new)
{
	t_listjb	*last;

	if (!new)
		return ;
	if (!(last = ft_lstlast_jb(*alst)))
		ft_lstadd_front_jb(alst, new);
	else
		last->next = new;
}
