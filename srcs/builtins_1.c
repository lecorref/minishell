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

int			echo_builtin(t_command *cmd)
{
	int		flag;
	int		i;

	i = 0;
	if (!cmd->command[i + 1])
	{
		ft_putchar_fd('\n', cmd->fd[1]);
		return (0);
	}
	flag = 0;
	while (!echo_n_parser(cmd->command[++i]))
		flag = 1;
	while (cmd->command[i] != NULL)
	{
		ft_putstr_fd(cmd->command[i], cmd->fd[1]);
		ft_putchar_fd(' ', cmd->fd[1]);
		i++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

/*
** if expand_tilde detects the ~ sign, it search the HOME env variable
** and join to it the path which can be typed behind, then return a malloc()
** pointer to this string. The string given as paramater is free.
** Otherwise, no tilde had been detected as first character, & it returns
** the string given as parameter, untouched.
*/
/*char	*expand_tilde(t_list **env, char *arg)
{
	char	*expanded;
	char	*env_home;
	int		i;

	i = 0;
	if (arg[i++] != '~')
		return (arg);
	env_home = find_env_value(env, "HOME");
	if (!(expanded = ft_strjoin(env_home, &arg[i])))
		return (NULL);
	free(arg);
	return (expanded);
}*/
char	*expand_tilde_and_exceptions(t_list **env, char *arg, t_command *cmd)
{
	char	*expanded;

	expanded = NULL;
	if ((ft_strcmp(arg, "-") == 0) || ((cmd->command[2]) &&
		(ft_strcmp(arg, "--") == 0 && ft_strcmp(cmd->command[2], "-") == 0)))
	{
		ft_putstr_fd(find_env_value(env, "OLDPWD"), 1);
		ft_putchar_fd('\n', cmd->fd[1]);
		if (!(expanded = ft_strdup(find_env_value(env, "OLDPWD"))))
			return (NULL);
	}
	else if (ft_strcmp(arg, "--") == 0 && !cmd->command[2])
	{
		if (!(expanded = ft_strdup(find_env_value(env, "HOME"))))
			return (NULL);
	}
	else if (arg[0] == '~')
	{
		if (!(expanded = ft_strjoin(find_env_value(env, "HOME"), &arg[1])))
			return (NULL);
	}
	else if (arg[0] != '~')
		return (arg);
	free(arg);
	return (expanded);
}

/*void	print_cd_error(char *cmd, char *arg, char *strerror, int fd)
{
	write(fd, "bash: ", 7);
	write(fd, cmd, ft_strlen(cmd));
	write(fd, ": ", 3);
	write(fd, arg, ft_strlen(arg));
	write(fd, ": ", 3);
	write(fd, strerror, ft_strlen(strerror));
	write(fd, "\n", 2);
}*/

int		update_pwd(t_list **env)
{
	char    *tmp;
	char    *pwd;
	char    *old_pwd;

	old_pwd = find_env_value(env, "PWD");
	if (!(tmp = ft_strjoin("OLDPWD=", old_pwd)))
		return (-1);
	add_env_variable(env, tmp);
	free(tmp);
	if (!(pwd = getcwd(NULL, 0)))
		return (-1);
	if (!(tmp = ft_strjoin("PWD=", pwd)))
		return (-1);
	add_env_variable(env, tmp);
	free(tmp);
	free(pwd);
	return (0);
}

/*
** cd uses the chdir func to sail into the filesystem & set errors.
** This builtin must specifically change the OLDPWD & the PWD variables; and
** handle the tilde char as well as the 'no' char, which means HOME directory.
*/
int		cd_builtin(t_list **env, t_command *cmd)
{
	if (!(cmd->command[1]))
		if (!(cmd->command[1] = ft_strjoin("~", "")))
			return (-1);
	if (!(cmd->command[1][0]))
		return (0);
	//cmd->command[1] = expand_tilde(env, cmd->command[1]);
	cmd->command[1] = expand_tilde_and_exceptions(env, cmd->command[1], cmd);
	if ((chdir(cmd->command[1])) == -1)
	{
//		print_cd_error(cmd->command[0], cmd->command[1], strerror(errno), 2);
		if (cmd->command[2])
		{
			if (cmd->command[1][0] == '-' && cmd->command[1][1] == '-')
				error_msg("bash", cmd, cmd->command[2], strerror(errno));//No such file or directory
			else
				error_msg("bash", cmd, NULL, "too many arguments");
		}
		else if (!cmd->command[2])
			error_msg("bash", cmd, cmd->command[1], strerror(errno));
		return (-1);
	}
	if (update_pwd(env) == -1)
		return (-1);
	return (0);
}
