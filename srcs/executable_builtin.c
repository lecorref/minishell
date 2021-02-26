/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/26 22:31:08 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** waitpid will wait until child process exits.
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

//	ft_array_string_del(cmd->command);// or a function like free_env (inside environment_2 file)???
	wstatus = 0;
	waitpid(fork_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		errno = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		errno = WTERMSIG(wstatus);
	signal(SIGQUIT, ctrl_back_slash_handler);
}

/*
** Command typed is not an absulute path, so in this function, an path from the
** $PATH env line (passed to the argument env_path, by the funtion
** path_to_executable) will be added to the command.
** The $PATH will be splitted by : to get an array of strings, with a path per
** line.
**
** 1. A slash will be joinned to the end of each env_path line, and then the
** command will be added after this slash, creatinf then an absolute path.
** 2. This absolute path will be tested by open(), if it does not open,
** variables will be freed. else, continue testing the other env_path lines
** untill one of them opens.
** 3. Return the absolute path that was opened (close its fd before the return)
** or NULL if cmd = NULL inside (then FT_strjoin returns NULL), or if the file
** does not open with any of the paths from $PATH (because then add_path_to_cmd
** will be freed and set to nULL by the ft_strdel function).
*/
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
	ft_strdel(&add_slash_to_path);
	ft_strdel(each_path_dir);
	if (fd > 2)
		close(fd);
	return (add_path_to_cmd);
}

/*
** Here the command typed will start with a ~/
** Same as find_absolute_path(), but here I duplicate the command but starting
** from it's index 1 (not 0 which is the tilde) and then I add "../.." to the
** beginning of the command.
**
** Ex: ~/../../bin/ls
** cmd_without_tilde = /../../bin/ls
** add_path_till_root_to_cmd = ../../../../bin/ls
*/
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
	ft_strdel(&cmd_without_tilde);
	ft_strdel(each_path_dir);
	if (fd > 2)
		close(fd);
	return (add_path_till_root_to_cmd);
}

/*
** 1. If command is not a absolute path:
**		Call find_absolute_path() funtion to handle it and return string "exit"
**		so I know it was not an absoulte path command that was typed, and then
**		I can output the correct error message.
**
** 2. If command starts with a ~/:
**		Call function relative_path() to handle it.
** If command is an absoulte path:
**		Duplicate command since it is already an absolute path.
** Open the absolute path command: Returns the string "exit_bash" so I know it
**	was an absoulte path command that was typed (or ~/), and then I can output
**	the correct error message.
**
** If all goes well, return the absolute path command (abs_path). either because
** it was already typed like that, or because it was turned into an absolute
** path.
*/
char	*path_to_executable(t_list **env, t_command *cmd)
{
	char	*abs_path;
	char	*env_path;
	int		fd;

	env_path = find_env_value(env, "PATH");
	if (!cmd->command)
		return (NULL);
	if (cmd->command[0][0] != '/' && ft_strncmp(cmd->command[0], "./", 2) != 0
		&& ft_strncmp(cmd->command[0], "../", 3) != 0 && env_path != NULL &&
		ft_strncmp(cmd->command[0], "~/", 2) != 0)
	{
		if ((abs_path = find_absolute_path(cmd->command[0], env_path)) == NULL)
			return ("exit");
	}
	else
	{
		if (ft_strncmp(cmd->command[0], "~/", 2) == 0)
			abs_path = relative_path(cmd->command[0], env_path);
		else
			abs_path = ft_strdup(cmd->command[0]);
		if ((fd = open(abs_path, O_RDONLY)) == -1)
			return ("exit_bash");
		if (fd > 2)
			close(fd);
	}
	return (abs_path);
}

/*
 * Here, enter has already been precessed (so ret_gnl = 1). So it'll wait for
** a SIGQUIT (ctrl\) and Quit (core dumped) if so.
**
**
** Fork - creates a new process (child) from the calling process (parent).
**
** On success:
**		the PID of the child process is returned in the parent, and 0 is
** returned in the child.
**		If execve fails, it checks the return of path_to_cmd() to see which
**		error message it will output.
**
** On failure:
**		-1 is returned in  the  parent, no child process is created, and
** errno is set appropriately.
*/
int		executable_builtin(t_list **env, t_command *cmd)
{
	char	**envir;
	char	*path_to_cmd;
	pid_t	fork_pid;

	envir = env_list_to_tab(*env);
//	envir = ft_split(find_env_value(env, "PATH"), ':');
	path_to_cmd = path_to_executable(env, cmd);
	signal(SIGQUIT, ctrl_back_slash_handler_quit);
////////////////// tests
	int	i;
	i = 0;
	while (cmd->command[i])
	{
		printf("cmd->command[%d] = %s\n", i, cmd->command[i]);
		i++;
	}
//	i = 0;
//	while (envir[i])
//		printf("env = %s\n", envir[i++]);
////////////////////
	if ((fork_pid = fork()) == -1)
		exit(errno);

	else if (fork_pid == 0)//child will exec
	{
		if (execve(path_to_cmd, cmd->command, envir) == -1)
		{
			if (ft_strcmp(path_to_cmd, "exit_bash") == 0)
			//{
				//errno = 2;
				error_msg("bash", cmd, NULL, strerror(2));
				//bash: <cmd>: No such file or directory (errno 2 for this msg)
			//}
			if (ft_strcmp(path_to_cmd, "exit") == 0)
			{
				printf("\n%s\n", path_to_cmd);
				error_msg(NULL, cmd, NULL, "command not found");
			}
				//<cmd>: command not found (no errno number for this message)
			free(path_to_cmd);
			exit(127);
		}
	}
	free(envir);
	parent_process(fork_pid);//child fork pid returned to the parent
	return (0);
}
//bash: <cmd>: No such file or directory (errno 2 for this message)
////<cmd>: command not found (there's no errno number for this message)
////child pid returned to the parent
