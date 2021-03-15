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

int			export_unset_error(t_list **env, t_command *cmd, t_list **export)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = -1;
	while (cmd->command[1][i] && (i < ft_strclen(cmd->command[1], '=')))
	{
		if (cmd->command[1][i] == '_')
			i++;
		else if ((ret = ft_isalnum((char)cmd->command[1][i])) == 0)
			break ;
		else
			i++;
	}
	if ((ft_isdigit(cmd->command[1][0]) || ret == 0 || cmd->command[1][0] == '='
		|| (ft_strcmp(&cmd->command[0][0], "unset") == 0 &&
		ft_strchr(&cmd->command[1][0], '='))) && cmd->command[1][0] != '-')
	{
		error_msg("bash", cmd, cmd->command[1], "not a valid identifier");
		g_exit_status = 1;
		return (RT_FAIL);
	}
	else if (ft_strcmp(&cmd->command[0][0], "export") == 0)
		export_builtin_arg(env, export, cmd);
	return (0);
}

int			unset_builtin(t_list **env, t_command *cmd, t_list **export)
{
	int		i;
	char	dest[1];

	i = 0;
	memset(dest, '\0', 2);
	update_underscore(env, last_arg(cmd));
	if (cmd->command[0] && cmd->command[1])
	{
		while (cmd->command[++i])
		{
			delete_env_variable(env, cmd->command[i]);
			delete_env_variable(export, cmd->command[i]);
			g_exit_status = 0;
		}
		if (export_unset_error(env, cmd, export) == RT_FAIL)
			return (RT_FAIL);
		else if (cmd->command[1][0] == '-')
		{
			error_msg("bash", cmd, ft_strncpy(dest, cmd->command[1], 2),
															"invalid option");
		//printf("export: usage: export [-fn] [name[=value] ...] or export -p\n");
			g_exit_status = 2;
		}
	}
	return (RT_SUCCESS);
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
/*static char	*cd_args(t_list **env, char *arg, t_command *cmd)
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
}*/

/*
** cd uses the chdir func to sail into the filesystem & set errors.
** This builtin must specifically change the OLDPWD & the PWD variables; and
** handle the tilde char as well as the 'no' char, which means HOME directory.
*/
//cmd->command[1] = expand_tilde(env, cmd->command[1]);

/*int		cd_builtin(t_list **env, t_command *cmd)
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
}*/
