/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/09 21:59:40 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include "../includes/minishell.h"

void	executable_parent(pid_t fork_pid)
{
	int	*wstatus;

	wstatus = 0;
	waitpid(fork_pid, wstatus, 0);
	if (WIFEXITED(*wstatus))//macro that returns true if the child terminated normally
		errno = WEXITSTATUS(*wstatus);//macro that returns the exit status of the child
	else if (WIFSIGNALED(*wstatus))// macro that returns true if the child process was terminated by a signal
		errno = WTERMSIG(*wstatus);//macro that returns the number of the signal that caused the child process to terminate
	signal(SIGQUIT, ctrl_d_handler);
}

// A child created via fork(2) inherits a copy of its parent's signal dispositions.  During an execve(2), the dispositions of handled  signals  are reset to the default; the dispositions of ignored signals are left unchanged.
//
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
	close(fd);
	return (add_path_to_cmd);
}

char	*path_to_executable(t_list **list, t_command *cmd)
{
	char	*absolute_path;
	char	*env_path;

	env_path = find_env_value(list, "PATH");
	printf("\n%s\n", env_path);
	if (!cmd->command)
		return (NULL);
	if (cmd->command[0][0] != '/' && ft_strncmp(cmd->command[0], "./", 2) != 0
		&& ft_strncmp(cmd->command[0], "../", 3) != 0 && env_path != NULL)
	{
		if ((absolute_path = find_absolute_path(cmd->command[0], env_path)) == NULL)
			error_message(cmd, ": command not found\n", 0);//1 = does not write "bash:"
	}
	else
	{
		if ((absolute_path = find_absolute_path(cmd->command[0], env_path)) == NULL)
			error_message(cmd, ": No such file or directory\n", 1);//0 = writes "bash:"
	}

	exit(127);//or exit(errno);?????
	return (absolute_path);
}

int		executable_builtin(t_list **head, t_command *cmd)
{
	char	**env;
	char	*path_to_cmd;
	pid_t	fork_pid;

	env = env_list_to_tab(*head);
	path_to_cmd = path_to_executable(head ,cmd);
	if ((fork_pid = fork()) == 0)//  On success, the PID of the child process is returned in the parent, and 0 is returned in the child
	{
		if (execve(path_to_cmd, cmd->command, env) == -1)
			error_message(cmd, "command not found", 1);
	}
	else if (fork_pid == -1)//On failure, -1 is returned in  the  parent, no child process is created, and errno is set appropriately.
		exit(errno);//or -1? or 2?
	else
		executable_parent(fork_pid);
	return (0);
}
