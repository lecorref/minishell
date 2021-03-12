#include "minishell.h"

int			env_builtin(t_list **env, t_command *cmd)
{
	if ((*env)->next == NULL)
		g_exit_status = 0;
	if ((*env)->next != NULL)
		env_builtin(&((*env)->next), cmd);
	ft_putstr_fd(ENV_KEY(*env), cmd->fd[1]);
	ft_putstr_fd(ENV_VALUE(*env), cmd->fd[1]);
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
static char	*cd_args(t_list **env, char *arg, t_command *cmd)
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

static int		update_pwd(t_list **env)
{
	char    *tmp;
	char    *pwd;
	char    *old_pwd;

	old_pwd = find_env_value(env, "PWD");
	if (!(tmp = ft_strjoin("OLDPWD=", old_pwd)))
		return (RT_FAIL);
	add_env_variable(env, tmp);
	free(tmp);
	if (!(pwd = getcwd(NULL, 0)))
		return (RT_FAIL);
	if (!(tmp = ft_strjoin("PWD=", pwd)))
		return (RT_FAIL);
	add_env_variable(env, tmp);
	free(tmp);
	free(pwd);
	return (RT_SUCCESS);
}

/*
** cd uses the chdir func to sail into the filesystem & set errors.
** This builtin must specifically change the OLDPWD & the PWD variables; and
** handle the tilde char as well as the 'no' char, which means HOME directory.
*/
//cmd->command[1] = expand_tilde(env, cmd->command[1]);

int		cd_builtin(t_list **env, t_command *cmd)
{
	if (!(cmd->command[1]))
		if (chdir(find_env_value(env, "HOME")) == -1)
			return (RT_FAIL);
	if (cmd->command[1])
		if (!(cmd->command[1] = cd_args(env, cmd->command[1], cmd)))
			return (RT_FAIL);
	update_underscore(env, last_arg(cmd));
	if (cmd->command[1] && chdir(cmd->command[1]) == -1)
	{
		if (cmd->command[2])
		{
			if (cmd->command[1][0] == '-' && cmd->command[1][1] == '-')
				error_msg("bash", cmd, cmd->command[2], strerror(errno));
			else
				error_msg("bash", cmd, NULL, "too many arguments");
		}
		else if (!cmd->command[2])
			error_msg("bash", cmd, cmd->command[1], strerror(errno));
		g_exit_status = 2;
		return (RT_SUCCESS);
	}
	if (update_pwd(env) != RT_SUCCESS)
		return (RT_FAIL);
	g_exit_status = 0;
	return (RT_SUCCESS);
}
