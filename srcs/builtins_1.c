#include "minishell.h"

void    echo_builtin(t_list **head, t_command *cmd)
{
	int		flag;

	flag = 0;
	(void)head;
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

void    env_builtin(t_list **head, t_command *cmd)
{
	char	**env;
	char	**ptr;

	env = env_list_to_tab(*head);
	ptr = env;
	while (*env != NULL)
	{
		ft_putstr_fd(*env, cmd->fd[1]);
		ft_putchar_fd('\n', cmd->fd[1]);
		free(*env);
		env++;
	}
	free(ptr);
}

void    export_builtin(t_list **head, t_command *cmd)
{
	if (cmd->args[0])
		add_env_variable(head, cmd->args[0]);
}

void    unset_builtin(t_list **head, t_command *cmd)
{
	if (cmd->args[0])
		delete_env_variable(head, cmd->args[0]);
}
