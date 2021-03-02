#include "minishell.h"

int			test_builtin(t_list **env, t_command *cmd)
{
	char	**env_tab;
	int		i;

	i = 0;
	if (!(env_tab = env_list_to_tab(*env)))
		return (-1);
	while (env_tab[i])
	{
		ft_putstr_fd(env_tab[i], cmd->fd[1]);
		ft_putchar_fd('\n', cmd->fd[1]);
		i++;
	}
	ft_freetab(env_tab);
	return (0);
}

/*
 ** while the next index array of the env command exists, compare the current
 ** index with the next one (meaning envir[i] is a char bigger than envir[i + 1].
 ** ex B, A), then, swap the lines.
 */
static char		**alpha_order_string(t_list **env)
{
	char	*envir_line;
	char	**env_tab;
	int		i;

	i = 0;
	envir_line = NULL;
	env_tab = env_list_to_tab(*env);
	if (!env_tab || !env_tab[1])
		return (NULL);
	while (env_tab[i + 1])
	{
		if (ft_strcmp(env_tab[i], env_tab[i + 1]) > 0)
		{
			envir_line = env_tab[i];
			env_tab[i] = env_tab[i + 1];
			env_tab[i + 1] = envir_line;
			i = 0;
		}
		else
			i++;
	}
	return (env_tab);
}

void	update_export_underscore(t_list **env, char *arg)
{
	char	*needle;
	int		len;

	if (!(needle = ft_strrchr(arg, '=')))
		len = ft_strlen(arg);
	else
		len = needle - arg;
	if (!(needle = ft_substr(arg, 0, len)))
		return ;
	update_underscore(env, needle);
	free(needle);
}

void		print_envir(t_command *cmd, char **envir, int i)
{
	char	*split_line;

	ft_putstr_fd("declare -x ", cmd->fd[1]);
	split_line = ft_substr(envir[i], 0, sizeof(char) *
			(ft_strclen(&envir[i][0], '=') + 1));
	ft_putstr_fd(split_line, cmd->fd[1]);
	ft_putchar_fd('\"', cmd->fd[1]);
	ft_putstr_fd(&envir[i][ft_strclen(envir[i], '=') + 1], cmd->fd[1]);
	ft_putstr_fd("\"\n", cmd->fd[1]);
	free(split_line);
}

int			export_builtin(t_list **env, t_command *cmd)
{
	char	**envir;
	int		i;

	if (cmd->command[0] && cmd->command[1])
	{
		update_export_underscore(env, cmd->command[1]);
		add_env_variable(env, cmd->command[1]);
	}
	else if (cmd->command[0] && !cmd->command[1])
	{
		if (!(envir = alpha_order_string(env)))
			return (-1);
		update_underscore(env, cmd->command[0]);
		i = -1;
		while (envir[++i] && envir[i + 1])
			print_envir(cmd, envir, i);
		ft_freetab(envir);
	}
	return (0);
}

int			unset_builtin(t_list **env, t_command *cmd)
{
	if (cmd->command[0] && cmd->command[1])
	{
		update_underscore(env, cmd->command[1]);
		delete_env_variable(env, cmd->command[1]);
	}
	else if (cmd->command[0] && !cmd->command[1])
		update_underscore(env, cmd->command[0]);
	return (0);
}
