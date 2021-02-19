#include "libft.h"

int		ft_lstsize_jb(t_listjb *lst)
{
	int	count;
	
	if (!lst)
		return (0);
	count = 1;
	while (lst->next)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
