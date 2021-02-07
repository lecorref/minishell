#include "minishell.h"

void    echo_builtin(t_list **env, t_command *cmd)
{
	int		flag;

	flag = 0;
	(void)env;
	if (!ft_strcmp(*(cmd->args), "-n"))
	{
		flag = 1;
		cmd->args++;
	}
	while (*(cmd->args) != NULL)
	{
		ft_putstr_fd(*(cmd->args), cmd->fd[1]);
		ft_putchar_fd(' ', cmd->fd[1]);
		cmd->args++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
}
