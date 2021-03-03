#include "minishell.h"

char	*last_arg(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->command[i])
		i++;
	if (i > 0)
		i -= 1;
	return (cmd->command[i]);
}

int		update_underscore(t_list **env, char *path_cmd)
{
	char	*keyvalue;

	if (!(keyvalue = ft_strjoin("_=", path_cmd)))
		return (1);
	add_env_variable(env, keyvalue);
	free(keyvalue);
	return (1);
}
