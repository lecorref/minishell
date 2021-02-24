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

int			echo_builtin(t_list **cmd)
{
	int		flag;
	int		i;

	i = 0;
	if (!CMD(*cmd)[i + 1])
	{
		ft_putchar_fd('\n', CMD_FD(*cmd)[1]);
		return (0);
	}
	flag = 0;
	while (!echo_n_parser(CMD(*cmd)[++i]))
		flag = 1;
	while (CMD(*cmd)[i] != NULL)
	{
		ft_putstr_fd(CMD(*cmd)[i], CMD_FD(*cmd)[1]);
		ft_putchar_fd(' ', CMD_FD(*cmd)[1]);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', CMD_FD(*cmd)[1]);
	return (0);
}

int			env_builtin(t_list **env, t_list **cmd)
{
	if ((*env)->next != NULL)
		env_builtin(&((*env)->next), cmd);
	ft_putstr_fd(ENV_KEY(*env), CMD_FD(*cmd)[1]);
	ft_putchar_fd('=', CMD_FD(*cmd)[1]);
	ft_putstr_fd(ENV_VALUE(*env), CMD_FD(*cmd)[1]);
	ft_putchar_fd('\n', CMD_FD(*cmd)[1]);
	return (0);
}

int			export_builtin(t_list **env, t_list **cmd)
{
	if (CMD(*cmd)[0] && CMD(*cmd)[1])
		add_env_variable(env, CMD(*cmd)[1]);
	if (CMD(*cmd)[0] && !(CMD(*cmd)[1]))
	{
		//print an environment different from **envp,
		//with "declare -x " before each lines
		return (0);
	}
	return (0);
}

int			unset_builtin(t_list **env, t_list **cmd)
{
	if (CMD(*cmd)[0] && CMD(*cmd)[1])
		delete_env_variable(env, CMD(*cmd)[1]);
	return (0);
}
