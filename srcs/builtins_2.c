#include "minishell.h"

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

//cd : if cd has no arguments, it must be replaced by the '~'. instead of what
//there would be a segfault (if command[1] == NULL) or
//an error (if *command[1] == '\0')

int    cd_builtin(t_list **head, t_command *cmd)
{
        char    *tmp;
        char    *pwd;
        char    *old_pwd;

        if ((chdir(cmd->command[1])) == -1)
        {
                tmp = strerror(errno);
                write(2, "bash: cd: ", 11);
                write(2, cmd->command[1], ft_strlen(cmd->command[1]));
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

/*
 * exit <nb less than long long>
 * output: exit\n
 * echo $? = if nb > 255 < long long, it starts over again to count from 0 to 255
 *
 * exit <any string or number bigger than a long long>
 * output: exit\nbash: exit: <argv>: numeric argument required\n
 * echo $? = 2
 *
 * exit <nb less than long long> <nb less than long long>
 * output: exit: too many arguments\n
 * IT DOES EXIT THE SHELL
 * echo $? = 1
 *
 * If <argv> is omitted, the exit status is that of the last command executed.
*/
int		exit_builtin(t_list **head, t_command *cmd)
{
	(void)head;
	(void)cmd;
	return (-1);
}
