/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/28 04:51:58 by jfreitas         ###   ########.fr       */
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
void	parent_process(pid_t fork_pid)
{
	int		wstatus;

	wstatus = 0;
	waitpid(fork_pid, &wstatus, 0);
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
** On failure:
**		-1 is returned in  the parent, no child process is created, and
** errno is set appropriately inside the function parent_process().
*/
int		executable_builtin(t_list **env, t_command *cmd)
{
	char	**envir;
	char	*path_to_cmd;
	pid_t	fork_pid;

	envir = env_list_to_tab(*env);
//	if (cmd->fd[3] != 0)
//		return (error_msg_2("y", cmd, cmd->file, strerror(cmd->fd[3])));
	if (!(path_to_cmd = path_to_executable(env, cmd)))
		return (127);
	signal(SIGQUIT, ctrl_back_slash_handler_quit);

/////////delete
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
/////////delete

	if ((fork_pid = fork()) == -1)
		exit(errno);
	else if (fork_pid == 0)
	{
		dup_fd(cmd->fd);
		if (execve(path_to_cmd, cmd->command, envir) == -1)
			error_msg("bash", cmd, NULL, strerror(2));
		exit(127);
	}
	fflush(stdout);
	free(path_to_cmd);
	clean_fd(cmd->fd);
	parent_process(fork_pid);
	ft_freetab(envir);
	return (0);
}
