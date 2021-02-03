/*#1. The adress of a pointer to a element.
#2. The adress of the function used to delete the content of l’élément.

Deletes and frees the given element and every successor of that element, using the function del and free(3)
Finally, the pointer to the list must be set to NULL.
*/
#include "../includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp_list;

	while (*lst)
	{
		tmp_list = (*lst)->next;
		(*del)((*lst)->content);
		free(*lst);
		*lst = tmp_list;
	}
	*lst = NULL;
}
