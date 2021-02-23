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
	char	**args;

	fdcp = ((t_command*)content)->fd;
	args = ((t_command*)content)->command;
	free(fdcp);
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
	free((t_command*)content);
}

void	ft_array_string_del(char **array)// or a function to free_env that Francois did (but to free a command typed that was passed to the struct (and then linked to the list).
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

