#include "minishell.h"

int				tokenize_error_pipe(t_list **head, char **pipeline,
									int i, int fd_tmp)
{
	if (fd_tmp != -1)
		close(fd_tmp);
	while (pipeline[i])
		free(pipeline[i++]);
	free(pipeline);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	return (RT_FAIL);
}

void			*tokenize_error_sc(t_list **head, char **array, char *line)
{
	ft_freetab(array);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	free(line);
	return (NULL);
}
