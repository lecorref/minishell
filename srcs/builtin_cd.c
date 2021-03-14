#include "minishell.h"

static int	update_pwd(t_list **env, char *cmd_arg)
{
	char	*tmp;
	char	*pwd;
	char	*old_pwd;

	old_pwd = find_env_value(env, "PWD");
	if (!(tmp = ft_strjoin("OLDPWD=", old_pwd)))
		return (RT_FAIL);
	add_env_variable(env, tmp);
	free(tmp);
	if (!(pwd = getcwd(NULL, 0)))
		return (GETCWD_ERR);
	if (!(tmp = ft_strjoin("PWD=", pwd)))
		return (RT_FAIL);
	if (cmd_arg && ft_strcmp(cmd_arg, "//") == 0)
	{
		free(tmp);
		tmp = ft_strdup("PWD=//");
	}
	add_env_variable(env, tmp);
	free(tmp);
	free(pwd);
	return (RT_SUCCESS);
}

int			cd_builtin(t_list **env, t_command *cmd)
{
	char	*path;
	int		err;

	err = 0;
	update_underscore(env, last_arg(cmd));
	path = check_options(env, cmd->command, cmd->fd, &err);
	if (err)
		return (cd_error(err, cmd->command));
	if (!path)
		path = cmd->command[1];
	if (chdir(path) == -1)
		if ((err = ERRNO_CD))
			return (cd_error(err, cmd->command));
	if ((err = update_pwd(env, cmd->command[1])) == RT_FAIL)
		return (RT_FAIL);
	if (err)
		return (cd_error(err, cmd->command));
	g_exit_status = 0;
	return (0);
}
