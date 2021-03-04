#include "minishell.h"

/*
** waitpid will wait until child process exits.
** Here the child fork pid is returned to the parent.
**
** setting uo errno:
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
void	parent_process(pid_t pid, t_command *cmd, char *pathcmd, char **env_tab)
{
	int		wstatus;

	wstatus = 0;
	printf("\n -- EXITED CHILD -- \n\n ->->->-> MINISHELL 0UTPUT ->->->->\n\n");
	free(pathcmd);
	ft_freetab(env_tab);
	close_fd(cmd->fd);
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		errno = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		errno = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
}

/*
 * Here, enter has already been precessed (so ret_gnl = 1). So it'll wait for
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
** errno is set appropriately inside the function parent_process().
*/

void	printthis(t_command *cmd, char *path_to_cmd)/////////delete
{
	printf("\n----------TESTING PURPOSES----------\n");
	int	i;
	i = 0;
	while (cmd->command[i])
	{
		printf("cmd->command[%d] = %s\n", i, cmd->command[i]);
		i++;
	}
	printf("path_to_cmd : %s\n", path_to_cmd);
	printf("----------TESTING PURPOSES----------\n\n");
	fflush(stdout);
}/////////delete

char	*last_arg(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->command[i])
		i++;
	if (i > 0)
		i -= 1;
	return (cmd->command[i]);
}

int		update_underscore(t_list **env, char *path_cmd)
{
	char	*keyvalue;

	if (!(keyvalue = ft_strjoin("_=", path_cmd)))
		return (1);
	add_env_variable(env, keyvalue);
	free(keyvalue);
	return (1);
}

int		executable_builtin(t_list **env, t_command *cmd)// iTS MORE THAN
{// 25 LINES JOY'LL CHANGE IT
	char	**env_tab;
	char	*path_to_cmd;
	pid_t	fork_pid;

	if (cmd->fd[3] != 0)
		return (error_msg_2("y", cmd, cmd->file, strerror(cmd->fd[3])));
	if (!(path_to_cmd = path_to_executable(env, cmd)))
		return (127);
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	env_tab = env_list_to_tab(*env);
	printthis(cmd, path_to_cmd);
	update_underscore(env, last_arg(cmd));
	if ((fork_pid = fork()) == -1)
		exit(errno);// check if its 2?
	else if (fork_pid == 0)
	{
		dup_fd(cmd->fd);
		if (execve(path_to_cmd, cmd->command, env_tab) == -1)
		{
			free(path_to_cmd);
			ft_freetab(env_tab);
			error_msg("bash", cmd, NULL, strerror(2));
		}
		errno = 127;// maybe it'll be something else, see later with $?
		return (-2);
	}
	parent_process(fork_pid, cmd, path_to_cmd, env_tab);
	return (0);
}
