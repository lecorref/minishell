#include "minishell.h"

int			env_builtin(t_list **env, t_command *cmd)
{
	if ((*env)->next != NULL)
		env_builtin(&((*env)->next), cmd);
	ft_putstr_fd(ENV_KEY(*env), cmd->fd[1]);
	ft_putchar_fd('=', cmd->fd[1]);
	ft_putstr_fd(ENV_VALUE(*env), cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

static char		**alpha_order_string(t_list *env)
{
	int		i;
	char	**envir;
	char	*envir_line;

	i = 0;
	envir = env_list_to_tab(env);
	if (!envir || !envir[1])
		return (NULL);
	while (envir[i + 1] != NULL)
	{
		if (ft_strcmp(envir[i], envir[i + 1]) > 0)// meaning envir[i] is a char bigger than envir[i + 1]. ex B, A .. then: swap
		{
			envir_line = envir[i];
			envir[i] = envir[i + 1];
			envir[i + 1] = envir_line;
			i = 0;
		}
		else
			i++;
	}
	return (envir);
}

int			export_builtin(t_list **env, t_command *cmd)
{
	char	**envir;
	char	*split_line;
	int		i;

	i = 0;
	envir = alpha_order_string(*env);
	if (cmd->command[0] && cmd->command[1])
		add_env_variable(env, cmd->command[1]);
	else if (cmd->command[0] && !cmd->command[1])
	{
		while (envir[i] && envir[i + 1] != NULL)
		{
			ft_putstr_fd("declare -x ", cmd->fd[1]);
			split_line = ft_substr(envir[i], 0, sizeof(char) *
									(ft_strclen(&envir[i][0], '=') + 1));
			ft_putstr_fd(split_line, cmd->fd[1]);
			ft_putchar_fd('\"', cmd->fd[1]);
			ft_putstr_fd(&envir[i][ft_strclen(envir[i], '=') + 1], cmd->fd[1]);
			ft_putstr_fd("\"\n", cmd->fd[1]);
			free(split_line);
			i++;
		}
		free(envir);
	}
	return (0);
}

int			unset_builtin(t_list **env, t_command *cmd)
{
	if (cmd->command[0] && cmd->command[1])
		delete_env_variable(env, cmd->command[1]);
	return (0);
}
