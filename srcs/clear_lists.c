#include "minishell.h"

void		clear_envlist(void *content)
{
	char	*keycp;
	char	*valuecp;

	keycp = ((t_env*)content)->key;
	valuecp = ((t_env*)content)->value;
	free(keycp);
	free(valuecp);
	free((t_env*)content);
}

void		clear_commandlist(void *content)
{
	int		i;
	int		*fdcp;
	char	*file;
	char	**args;
	char	*unexpanded;

	fdcp = ((t_command*)content)->fd;
	args = ((t_command*)content)->command;
	file = ((t_command*)content)->file;
	unexpanded = ((t_command*)content)->unexpanded;
	close_fd(fdcp);
	free(unexpanded);
	free(fdcp);
	i = -1;
	if (args)
		ft_freetab(args);
	if (file)
		free(file);
	free((t_command*)content);
}

void			clear_arglist(void *content)
{
	(void)content;
}

int			clear_lists_exit(t_list **cmd, t_list **env, char *saved_path)
{
	free(saved_path);
	ft_lstclear(cmd, &clear_commandlist);
	ft_lstclear(env, &clear_envlist);
	return (RT_EXIT);
}

// or a function to free_env that Francois did (but to free a command typed that
// was passed to the struct (and then linked to the list).
void	ft_array_string_del(char **array)
{
	int i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
