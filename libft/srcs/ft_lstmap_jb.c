#include "libft.h"

/*
** Itère sur la liste lst et applique la fonction f au contenu de chaque
** élément. Crée une nouvelle liste résultant des applications successives de f.
** la fonction del est la pour detruire le contenu d un element si necessaire
*/


t_listjb		*ft_lstmap_jb(t_listjb *lst, void *(*f)(void *), void (*del)(void *))
{
	t_listjb	*new;
	t_listjb	*tmp;

	if (!lst || !f || !del)
		return (NULL);
	new = NULL;
	while (lst)
	{
		if (!(tmp = ft_lstnew_jb((*f)(lst->content))))
		{
			ft_lstclear_jb(&new, (*del));
			return (NULL);
		}
		ft_lstadd_back_jb(&new, tmp);
		lst = lst->next;
	}
	return (new);
}

/*
** la fonction (*f) fait une modif de lst->content (void*) et renvoie
** ce lst->content (void*) qui va être copié dans le nouveau
** maillon par ft_lstnew. Ce maillon nouvellement créé va être ajouté
** en fin de liste par ft_lstadd_back et faire pointer le pointeur head
** sur le début de liste.
*/
