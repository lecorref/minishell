#include "minishell.h"

/*
** waitpid will wait until child process exits.
** Here the child fork pid is returned to the parent.
**
** setting up error:
** WIFEXITED(wstatus) - macro that returns true if the child terminated normally
** WEXITSTATUS(wstatus) - macro that returns the exit status of the child
** WIFSIGNALED(wstatus) - macro that returns true if the child process was
** terminated by a signal
** WTERMSIG(wstatus) - macro that returns the number of the signal that caused
** the child process to terminate
**
** waiting for a SIGQUIT (ctrl\) signal if any, while child does not quit (if
** it gets to quit).
*/
static int	parent_process(pid_t pid, char *pathcmd, char **env_tab)
{
	int		wstatus;

	free(pathcmd);
	ft_freetab(env_tab);
	wstatus = 0;
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		g_exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		g_exit_status = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
	return (RT_SUCCESS);
}

/*
** Here, enter has already been precessed (so ret_gnl = 1). So it'll wait for
** a SIGQUIT (ctrl\) and Quit (core dumped) if so.
**
** Fork - creates a new process (child) from the calling process (parent).
**
** On success:
**		the PID of the child process is returned in the parent, and 0 is
** returned in the child.
**		child process fork will execute things here.
**		Execve will receive only absolute path executables, that will be either
**		input by the user, or transformed into an aboslute path executable by
**		the functions inside the file executable_builtin_path.c
**		If execve fails, it outputs an error message.
**		exit 127 to exit the child process
**
**		OBS.: I think that as "the child process and the parent process run in
**		separate memory spaces"), the memory is also duplicated, therefore it
**		needs to be freed inside the child too (and then also on the parent).
**
** On failure:
**		-1 is returned in  the parent, no child process is created, and
** error number is set appropriately inside the function parent_process().
*/

static int	fork_extern(t_command *cmd, char *path_to_cmd, char **env_tab)
{
	int	cpid;

	//printf("path_to_cmd execve: |%s|\n%s", path_to_cmd, LINE2);//TEST DEL LATER
	if ((cpid = fork()) == -1)
	{
		free(path_to_cmd);
		ft_freetab(env_tab);
		g_exit_status = 2;
		return (RT_SUCCESS);
	}
	else if (cpid == 0)
	{
		dup_fd(cmd->fd);
		if (execve(path_to_cmd, cmd->command, env_tab) == -1)
		{
			error_msg("bash", cmd, NULL, strerror(errno));
			free(path_to_cmd);
			ft_freetab(env_tab);
		}
		if (errno == EACCES)
			g_exit_status = 126;
		else if (errno == ENOENT)
			g_exit_status = 127;
		return (RT_FAIL);
	}
	else
		return (cpid);
}

/*
** Ex of command that is not an absolute or relative path: ls lo minishell
**		for those, a path needs to be joined to it.
** Ex of command that is an absolute or relative path: ~/ /bin/ls ./minishell
**		for ~/ a path needs to be joined to it.
**		for the rest, just duplicate it.
**
** Returns the absolute path command/executable (abs_path). either because it
** was already typed like that, or because it was turned into an absolute path
** by the funtion relative_path() or absolute_path().
** Returns a malloc string, so it needs to be freed later on.
*/
static char	*path_to_executable(t_list **env, t_command *cmd, char *saved_path)
{
	char	*abs_path;
	char	*home_path;
	char	*path;
	char	**split_path;

	abs_path = NULL;
	home_path = find_env_value(env, "HOME");
	path = find_env_value(env, "PATH");
	if (!ft_strchr(&cmd->command[0][0], '/') && cmd->command[0][0] != '.'
		&& ft_strncmp(cmd->command[0], "~/", 2) != 0)
	{
		if (!path || (!path && !saved_path))
		{
			error_msg("bash", cmd, NULL, strerror(2));
			return ("");
		}
		if (!(split_path = ft_split_jb(path, ':')))
			return (NULL);
		abs_path = relative_path(cmd, split_path, saved_path);
		ft_freetab(split_path);
	}
	else
		abs_path = absolute_path(cmd, home_path);
	return (abs_path);
}

static int	check_special_case(t_command *cmd)
{
	if (!(cmd->command[0]) || !(cmd->command[0][0]))
	{
		g_exit_status = 0;
		return (RT_SUCCESS);
	}
	return (1);
}

int			execute_extern(t_list **env, t_command *cmd, char *saved_path)
{
	char	**env_tab;
	char	*path_to_cmd;
	pid_t	fork_pid;

	if (check_special_case(cmd) == RT_SUCCESS)
		return (RT_SUCCESS);
	if (!(path_to_cmd = path_to_executable(env, cmd, saved_path)))
		return (RT_FAIL);
	if (!*path_to_cmd)
	{
		g_exit_status = 127;
		return (RT_SUCCESS);
	}
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	if (!(env_tab = env_list_to_tab(*env)))
		return (RT_FAIL);
	update_underscore(env, last_arg(cmd));
	if ((fork_pid = fork_extern(cmd, path_to_cmd, env_tab)) == RT_FAIL)
		return (RT_FAIL);
	return (parent_process(fork_pid, path_to_cmd, env_tab));
}
