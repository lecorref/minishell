#include "minishell.h"

static int	echo_n_parser(char *str)
{
	if (*str != '-')
		return (1);
	while (*(str + 1))
	{
		if (*(str + 1) != 'n')
			return (1);
		str++;
	}
	return (0);
}

int			echo_builtin(t_list **env, t_command *cmd)
{
	int		flag;

	flag = 0;
	(void)env;
	cmd->command++;
	while (!echo_n_parser(*(cmd->command)))
	{
		flag = 1;
		cmd->command++;
	}
	while (*(cmd->command) != NULL)
	{
		ft_putstr_fd(*(cmd->command), cmd->fd[1]);
		ft_putchar_fd(' ', cmd->fd[1]);
		cmd->command++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

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

int			export_builtin(t_list **env, t_command *cmd)
{
	if (cmd->command[0] && cmd->command[1])
		add_env_variable(env, cmd->command[1]);
	if (cmd->command[0] && !(cmd->command[1]))
	{
		//print an environment different from **envp,
		//with "declare -x " before each lines
		return (0);
	}
	return (0);
}

int			unset_builtin(t_list **env, t_command *cmd)
{
	if (cmd->command[0] && cmd->command[1])
		delete_env_variable(env, cmd->command[1]);
	return (0);
}
