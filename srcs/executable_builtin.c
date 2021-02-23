/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/23 17:04:56 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*//macro that returns the exit status of the child
 * //macro that returns true if the child terminated normally
  macro that returns true if the child process was terminated by a signal
 //macro that returns the number of the signal that caused the child process
 //to terminate
*/

void	executable_parent(t_command *cmd, pid_t fork_pid)
{
	int		wstatus;

	if (cmd->command[0])
		;
	//free(cmd->command[0]);//later to be replaced for a function that frees 
	wstatus = 0;//the t_cmd (or at least the **command?)
	waitpid(fork_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		errno = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		errno = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
}

// A child created via fork(2) inherits a copy of its parent's signal
// dispositions.  During an execve(2), the dispositions of handled  signals  are
// reset to the default; the dispositions of ignored signals are left unchanged.
char	*find_absolute_path(char *cmd, char *env_path)
{
	char	**each_path_dir;
	char	*add_slash_to_path;
	char	*add_path_to_cmd;
	int		i;
	int		fd;

	i = -1;
	fd = 0;
	each_path_dir = ft_split(env_path, ':');
	while (each_path_dir[++i])
	{
		add_slash_to_path = ft_strjoin(each_path_dir[i], "/");
		add_path_to_cmd = ft_strjoin(add_slash_to_path, cmd);
		if ((fd = open(add_path_to_cmd, O_RDONLY)) == -1)
		{
			ft_strdel(&add_slash_to_path);
			ft_strdel(&add_path_to_cmd);
		}
		else
			break ;
	}
	ft_strdel(each_path_dir);
	if (fd == 1)
		close(fd);
	return (add_path_to_cmd);
}

char	*relative_path(char *cmd, char *env_path)
{
	char	**each_path_dir;
	char	*cmd_without_tilde;
	char	*add_path_till_root_to_cmd;
	int		i;
	int		fd;

	i = -1;
	fd = 0;
	each_path_dir = ft_split(env_path, ':');
	while (each_path_dir[++i])
	{
		cmd_without_tilde = ft_strdup(&cmd[1]);
		add_path_till_root_to_cmd = ft_strjoin("../..", cmd_without_tilde);
		if ((fd = open(add_path_till_root_to_cmd, O_RDONLY)) == -1)
		{
			ft_strdel(&cmd_without_tilde);
			ft_strdel(&add_path_till_root_to_cmd);
		}
		else
			break ;
	}
	ft_strdel(each_path_dir);
	if (fd == 1)
		close(fd);
	return (add_path_till_root_to_cmd);
}

char	*path_to_executable(t_list **env, t_command *cmd)
{
	char	*absolute_path;
	char	*env_path;

	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	env_path = find_env_value(env, "PATH");
	if (!cmd->command)
		return (NULL);
	if (cmd->command[0][0] != '/' && ft_strncmp(cmd->command[0], "./", 2) != 0
		&& ft_strncmp(cmd->command[0], "../", 3) != 0 && env_path != NULL &&
		ft_strncmp(cmd->command[0], "~/", 2) != 0)
	{
		if ((absolute_path = find_absolute_path(cmd->command[0], env_path)) == NULL)
			return ("exit");
	}
	else
	{
		if (ft_strncmp(cmd->command[0], "~/", 2) == 0)
			absolute_path = relative_path(cmd->command[0], env_path);
		else
			absolute_path = ft_strdup(cmd->command[0]);
		if (open(absolute_path, O_RDONLY) == -1)
			return ("exit_bash");
	}
	return (absolute_path);
}

/*
 * Fork:
 * On success, the PID of the child process is returned in the parent, and 0 is returned in the child.
 * On failure, -1 is returned in  the  parent, no child process is created, and errno is set appropriately.
 */
int		executable_builtin(t_list **env, t_command *cmd)
{
	char	**envir;
	char	*path_to_cmd;
	pid_t	fork_pid;

	envir = env_list_to_tab(*env);
	path_to_cmd = path_to_executable(env ,cmd);
	if ((fork_pid = fork()) == -1)
		exit(errno);// or -1???
	else if (fork_pid == 0)//child
	{
		if (execve(path_to_cmd, cmd->command, envir) == -1)
		{
			if (ft_strcmp(path_to_cmd, "exit_bash") == 0)
			{
				errno = 2;
				error_msg_bash(cmd, -1, "");//bash: <cmd>: No such file or directory (errno 2 for this message)
			}
			if (ft_strcmp(path_to_cmd, "exit") == 0)
				error_msg(cmd, 127, "command not found\n");//<cmd>: command not found (there's no errno number for this message)
			exit(127);
		}
	}
	else
		executable_parent(cmd, fork_pid);//child pid returned to the parent
	return (0);
}
