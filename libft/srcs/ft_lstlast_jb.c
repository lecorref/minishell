#include "libft.h"

t_listjb	*ft_lstlast_jb(t_listjb *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
