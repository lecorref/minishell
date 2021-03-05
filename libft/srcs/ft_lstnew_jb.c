#include "libft.h"

t_listjb		*ft_lstnew_jb(void *content)
{
	t_listjb  *newlink;

	if (!(newlink = (t_listjb*)malloc(sizeof(t_listjb))))
		return (NULL);
	newlink->content = content;
	newlink->next = NULL;
	return (newlink);
}
