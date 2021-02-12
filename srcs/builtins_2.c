#include "minishell.h"

int		pwd_builtin(t_list **head, t_command *cmd)
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

char	*expand_tilde(t_list **head, char *arg)
{
	char	*expanded;
	char	*env_home;
	int		i;

	i = 0;
	if (arg[i] == '~')
	{
		while (arg[++i])
			if (arg[i] != '/')
				return (arg);
		env_home = find_env_value(head, "HOME");
		if (!(expanded = (char*)malloc(sizeof(char) * (ft_strlen(env_home) + 1))))
			return (NULL);
		ft_strlcpy(expanded, env_home, ft_strlen(env_home) + 1);
		free(arg);
		return (expanded);
	}
	return (arg);
}

int		cd_builtin(t_list **head, t_command *cmd)
{
	char    *tmp;
	char    *pwd;
	char    *old_pwd;

	cmd->command[1] = expand_tilde(head, cmd->command[1]);
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

int		exit_arg(t_command *cmd, size_t i)
{
	int	errnb;

	errnb = 0;
	if (i == ft_strlen(cmd->command[1]))
	{
		if (!cmd->command[2])
		{
			errno = ft_atoi(cmd->command[1]);
			errno += 256;
			errno %= 256;
			exit(errno);
		}
		else if (cmd->command[2])
		{
			errnb = 1;
			error_msg_bash(cmd, errnb, "too many arguments\n");//too many arguments
			return (errnb);// or 131?
		}
	}
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
 * exit <str or nb less than long long> <str or nb less than long long>
 * output: exit\nbash: exit: too many arguments\n
 * IT DOES EXIT THE SHELL
 * echo $? = 1
 *
 * If <argv> is omitted, the exit status is that of the last command executed.
*/
int		exit_builtin(t_list **head, t_command *cmd)
{
	size_t	i;
	int	errnb;

	(void)head;
	errnb = 0;
	i = 0;
	ft_putstr_fd("exit\n", 2);
	if (cmd->command[1])
	{
		if (cmd->command[1][0] == '+' || cmd->command[1][0] == '-')
			i++;
		while (ft_isdigit((char)cmd->command[1][i]) == 1)
			i++;
		if (i != ft_strlen(cmd->command[1]))
		{
			errnb = 2;
			error_msg_bash(cmd, errnb, cmd->command[1]);//numeric argument required
			ft_putstr_fd(": numeric argument required\n", cmd->fd[2]);
			exit (errnb);
		}
		exit_arg(cmd, i);
	}
	else if (!cmd->command[1])
		exit (0);
	return (0);
}
