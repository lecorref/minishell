/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 01:35:31 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/06 00:22:47 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int		parent_process(pid_t pid, char *pathcmd, char **env_tab)
{
	int		wstatus;

	free(pathcmd);
	ft_freetab(env_tab);
	wstatus = 0;
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		errno = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		errno = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
	return (errno);
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
** errno is set appropriately inside the function parent_process().
*/

int		fork_extern(t_command *cmd, char *path_to_cmd, char **env_tab)
{
	int	cpid;

	printf("\tpath_to_cmd (to execve) : |%s|\n", path_to_cmd);//TEST-TO DEL LATER
	if ((cpid = fork()) == -1)
		exit(errno);// check if its 2?
	else if (cpid == 0)
	{
		dup_fd(cmd->fd);
		if (execve(path_to_cmd, cmd->command, env_tab) == -1)
		{
			free(path_to_cmd);
			ft_freetab(env_tab);
			error_msg("bash", cmd, NULL, strerror(2));
		}
		//errno = 127;
		//return (-2);
	//	ft_lstclear(cmd, &clear_commandlist); DELETE THINGS BEFORE EXIT
	//	ft_lstclear(env, &clear_envlist); DELETE THINGS BEFORE EXIT
		exit(127);//I think, as it's a child which would fail & not the main
	}//pgm (parent), that it would be better to exit() it ?
	else
		return (cpid);
}

int		execute_extern(t_list **env, t_command *cmd)
{
	char	**env_tab;
	char	*path_to_cmd;
	pid_t	fork_pid;

	if (cmd->fd[3] != 0)
		return (error_msg_2("y", cmd, cmd->file, strerror(cmd->fd[3])));
	if (!(path_to_cmd = path_to_executable(env, cmd)))
		return (127);
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	env_tab = env_list_to_tab(*env);
	update_underscore(env, last_arg(cmd));
	fork_pid = fork_extern(cmd, path_to_cmd, env_tab);
	return (parent_process(fork_pid, path_to_cmd, env_tab));
	//return (0);
}
