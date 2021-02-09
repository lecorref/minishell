#include "../includes/minishell.h"

int	    pwd_builtin(t_list **head, t_command *cmd)
{
	char	*stored;

	(void)head;
	stored = getcwd(NULL, 0);
	ft_putstr_fd(stored, cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	free(stored);
	return (0);
}

int	    exit_builtin(t_list **head, t_command *cmd)
{
	(void)head;
	(void)cmd;
	return (-1);
}

int    cd_builtin(t_list **head, t_command *cmd)
{
        char    *tmp;
        char    *pwd;
        char    *old_pwd;

        if ((chdir(cmd->command[0])) == -1)
        {
                tmp = strerror(errno);
                write(2, "bash: cd: ", 11);
                write(2, cmd->command[0], ft_strlen(cmd->command[0]));
                write(2, ": ", 3);
                write(2, tmp, ft_strlen(tmp));
                write(2, "\n", 2);
                return (-1);
        }
        old_pwd = find_env_value(head, "PWD");
        tmp = ft_strjoin("OLDPWD=", old_pwd);
        add_env_variable(head, tmp);
        free(tmp);
        pwd = getcwd(NULL, 0);
        tmp = ft_strjoin("PWD=", pwd);
        add_env_variable(head, tmp);
        free(tmp);
        free(pwd);
		return (0);
}
