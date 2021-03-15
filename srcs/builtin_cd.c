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

int			check_fd_n_path(char *path, t_command *cmd, int *err)
{
	struct stat	fd_check;
	struct stat	path_check;

	stat(path, &path_check);
	if (!(S_ISDIR(path_check.st_mode)))
	{
		*err = ERRNO_CD;
		return (1);
	}
	fstat(cmd->fd[1], &fd_check);
	if (S_ISFIFO(fd_check.st_mode))
	{
		*err = CD_FIFO;
		return (1);
	}
	fstat(cmd->fd[0], &fd_check);
	if (S_ISFIFO(fd_check.st_mode))
	{
		*err = CD_FIFO;
		return (1);
	}
	return (0);
}

int			cd_builtin(t_list **env, t_command *cmd)
{
	char	*path;
	char	cwd[300];
	int		err;

	update_underscore(env, last_arg(cmd));
	path = check_options(env, cmd->command, cmd->fd, &err);
	if (err)
		return (cd_error(err, cmd->command[1]));
	if (!path)
		path = cmd->command[1];
	ft_memset(cwd, 0, sizeof(cwd));
	if (!*path)
		if (!(path = getcwd(cwd, 300)) && (err == GETCWD_ERR))
			return (cd_error(err, cmd->command[1]));
	if (check_fd_n_path(path, cmd, &err))
		return (cd_error(err, path));
	if (chdir(path) == -1)
		if ((err = ERRNO_CD))
			return (cd_error(err, path));
	if ((err = update_pwd(env, cmd->command[1])) == RT_FAIL)
		return (RT_FAIL);
	if (err)
		return (cd_error(err, path));
	g_exit_status = 0;
	return (0);
}
