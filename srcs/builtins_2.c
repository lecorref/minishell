#include "../includes/minishell.h"

int	    pwd_builtin(t_list **head, t_command *cmd)
{
	char	*stored;

	(void)head;
	stored = getcwd(NULL, 0);
	ft_putstr_fd(stored, cmd=>fd[1]);
	ft_putchar_fd('\n', cmd=>fd[1]);
	free(stored);
	return (0);
}

int	    cd_builtin(t_list **head, t_command *cmd)
{
	chdir(cmd->command[1]);
	return (0);
}

int	    exit_builtin(t_list **head, t_command *cmd)
{
	(void)head;
	(void)cmd;
	return (-1);
}
