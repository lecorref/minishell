/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/12 21:36:12 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include "../includes/minishell.h"

//////////////////////////////// STILL WORKING ON IT ///////////////////////////
void	executable_parent(pid_t fork_pid)
{
	int		wstatus;

	wstatus = 0;
	waitpid(fork_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))//macro that returns true if the child terminated normally
		errno = WEXITSTATUS(wstatus);//macro that returns the exit status of the child
	else if (WIFSIGNALED(wstatus))// macro that returns true if the child process was terminated by a signal
		errno = WTERMSIG(wstatus);//macro that returns the number of the signal that caused the child process to terminate
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
	each_path_dir = ft_strsplit(env_path, ':');
	add_path_to_cmd = NULL;
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

char	*path_to_executable(t_list **list, t_command *cmd)
{
	char	*absolute_path;
	char	*env_path;

	env_path = find_env_value(list, "PATH");
	if (!cmd->command)
		return (NULL);
	if (cmd->command[0][0] != '/' && ft_strncmp(cmd->command[0], "./", 2) != 0
		&& ft_strncmp(cmd->command[0], "../", 3) != 0 && env_path != NULL &&
		ft_strncmp(cmd->command[0], "~/", 2) != 0)
	{
		if ((absolute_path = find_absolute_path(cmd->command[0], env_path)) == NULL)
		{
		//	error_msg(cmd, -1, "");//0 = does not write "bash:"
			return ("exit");
		}
	}
	else
	{
		absolute_path = ft_strdup(cmd->command[0]);
		if (open(cmd->command[0], O_RDONLY) == -1)
		{
		//	error_msg_bash(cmd, -1, "");//1 = writes "bash:"
			return ("exit_bash");
		}
	}
//	exit(127);//or exit(errno);????? NOT HERE
	return (absolute_path);
}

int		executable_builtin(t_list **head, t_command *cmd)
{
	char	**env;
	char	*path_to_cmd;
	pid_t	fork_pid;

	env = env_list_to_tab(*head);
	path_to_cmd = path_to_executable(head ,cmd);
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	if ((fork_pid = fork()) == -1)//On failure, -1 is returned in  the  parent, 
		exit(2);//or -1? or errno?//no child process is created, and errno is set appropriately.
	else if (fork_pid == 0)//  On success, the PID of the child process is
	{//returned in the parent, and 0 is returned in the child
		if (execve(path_to_cmd, cmd->command, env) == -1)
		{
			if (ft_strcmp(path_to_cmd, "exit_bash") == 0)
				error_msg_bash(cmd, 127, "");//bash: /bin/lo: No such file or directory
			if (ft_strcmp(path_to_cmd, "exit") == 0)
				error_msg(cmd, 127, "");//lo: command not found
		}
	}
	else
	{
		free(cmd->command[0]);
		//later to be replaced for a function that frees the t_cmd
		//(or at least the **command?)
		executable_parent(fork_pid);
	}
	return (0);
}
