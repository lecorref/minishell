/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/22 21:35:34 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* WIFEXITED(wstatus) - macro that returns true if the child terminated normally
 * WEXITSTATUS(wstatus) - macro that returns the exit status of the child
 * WIFSIGNALED(wstatus) - macro that returns true if the child process was
 * terminated by a signal
 * WTERMSIG(wstatus) - macro that returns the number of the signal that caused
 * the child process to terminate
 */
void	executable_parent(t_list **cmd, pid_t fork_pid)
{
	int		wstatus;

//	printf("ENTERED PARENT\n");
	free(CMD(*cmd)[0]);//later to be replaced for a function that frees the t_cmd (or at least the **command?)
	wstatus = 0;
	waitpid(fork_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		errno = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		errno = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
}

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

char	*path_to_executable(t_list **env, t_list **cmd)
{
	char	*abs_path;
	char	*env_path;

	signal(SIGQUIT, ctrl_back_slash_handler_quit);
	env_path = find_env_value(env, "PATH");
	if (!CMD(*cmd))
		return (NULL);
	if (CMD(*cmd)[0][0] != '/' && ft_strncmp(CMD(*cmd)[0], "./", 2) != 0
		&& ft_strncmp(CMD(*cmd)[0], "../", 3) != 0 && env_path != NULL &&
		ft_strncmp(CMD(*cmd)[0], "~/", 2) != 0)
	{
		if ((abs_path = find_absolute_path(CMD(*cmd)[0], env_path)) == NULL)
			return ("exit");
	}
	else
	{
		if (ft_strncmp(CMD(*cmd)[0], "~/", 2) == 0)
			abs_path = relative_path(CMD(*cmd)[0], env_path);
		else
			abs_path = ft_strdup(CMD(*cmd)[0]);
		if (open(abs_path, O_RDONLY) == -1)
			return ("exit_bash");
	}
	return (abs_path);
}

/* Fork:
 * On success, the PID of the child process is returned in the parent, and 0 is
 * returned in the child.
 * On failure, -1 is returned in  the  parent, no child process is created, and
 * errno is set appropriately.
 *
 * A child created via fork(2) inherits a copy of its parent's signal
 * dispositions.  During an execve(2), the dispositions of handled  signals are
 * reset to the default; the dispositions of ignored signals are left unchanged
 */
int		executable_builtin(t_list **env, t_list **cmd)
{
	char	**envir;
	char	*path_to_cmd;
	pid_t	fork_pid;

	envir = env_list_to_tab(*env);
	path_to_cmd = path_to_executable(env, cmd);


	int i;

	i = 0;
	while (CMD(*cmd)[i])
	{
		printf("cmd[%d] = %s\n", i,  CMD(*cmd)[i]);
		i++;
	}
//	i = 0;
//	while (envir[i])
//	{
//		printf("env = %s\n", envir[i]);
//		i++;
//	}
	printf("path to cmd = %s\n", path_to_cmd);

	if ((fork_pid = fork()) == -1)
	{
	//	printf("CHILD FORK = -1\n");
		exit(errno);//or somethimng else?
	}
	else if (fork_pid == 0)//child
	{
		char *argv[] = {"ls", "-la", 0};// works with NULL OR 0, BUT NOT WITH ""  or ''

	//	printf("CHILD FORK = 0\n");
		if (execve(path_to_cmd, argv, envir) == -1)
		{
		//	printf("EXECVE = -1\n");
			if (ft_strcmp(path_to_cmd, "exit_bash") == 0)
			{
				errno = 2;
				error_msg_bash(cmd, -1, "");
				//bash: <cmd>: No such file or directory (errno 2 for this msg)
			}
			if (ft_strcmp(path_to_cmd, "exit") == 0)
				error_msg(cmd, 127, "command not found\n");
				//<cmd>: command not found (no errno number for this message)
			exit(127);
		}
	}
	else
		executable_parent(cmd, fork_pid);//child pid returned to the parent
	return (0);
}
