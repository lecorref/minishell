/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_builtin_path.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/28 04:16:27 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** It opens the directories passed as absolute paths to the argum each_path_dir.
** Then it reads this directory and returns a pointer to a structure called
** dirent. One of the variables inside this struc is d_name which contains
** the filename.
** The filename(s) inside the directory is compared with the executable input
** by the user, and if the comparison is equal to 0, then this executable
** indeed exists inside this directory (return 0). Otherwise, there is not
** executable with the given name inside this directoy (return -1).
*/
int			test_cmd(char *each_path_dir, char *executable)
{
	int				cmp;
	DIR				*dp;
	struct dirent	*dirp;

	if (!(dp = opendir(each_path_dir)))
		return (-1);
	while ((dirp = readdir(dp)))
		if (!(cmp = ft_strcmp(executable, dirp->d_name)))
			break;
	closedir(dp);
	if (cmp == 0)
		return (0);
	return (-1);
}

/*
** Joining a / to the end of the absolute path (this being one of the paths
** inside of $PATH, or the PWD absolute path).
** Then joining the executable typed by the user to it and returning the
** complete absolute path.
*/
char	*add_path_to_cmd(char *abs_path, char *executable)
{
	char	*add_slash;
	char	*add_path;

	if (!(add_slash = ft_strjoin(abs_path, "/")))
		return (NULL);
	if (!(add_path = ft_strjoin(add_slash, executable)))
		return (NULL);
	ft_strdel(&add_slash);
	return (add_path);
}
/*
** Command typed is not an absulute path, so in this function, a path from the
** $PATH or the $PWD env line will be added to the command.
** The $PATH will be splitted by : to get an array of strings, with a path per
** line.
**
** 1. Testing the executable with the $PWD path, if the executable is found,
** call the function add_path_to_cmd() and return the absolute path returned
** from it.
** 2. loop to test the executable with each $PATH path, until the test
** return is equal 0, if so break the loop.
** 3. Otherwise if the test with $PATH returned 0, call the function
** add_path_to_cmd().
** 4. if both $PATH and $PWD tests returned -1, it means that the executable
** input was not found in any directory, then,  display the error message.
*/
char	*relative_path(t_command *cmd, char **each_path_dir, char *pwd_path)
{
	char	*add_path;
	int		ret_pwd_path;
	int		ret_env_path;
	int		i;

	i = -1;
	ret_env_path = -1;
	add_path = ft_strdup("");
	if ((ret_pwd_path = test_cmd(pwd_path, cmd->command[0])) == 0)
	{
		add_path = add_path_to_cmd(pwd_path, cmd->command[0]);
		return (add_path);
	}
	while (each_path_dir[++i])
		if ((ret_env_path = test_cmd(each_path_dir[i], cmd->command[0])) == 0)
			break ;
	if (ret_env_path == 0)
		add_path = add_path_to_cmd(each_path_dir[i], cmd->command[0]);
	else if (ret_env_path == -1 && ret_pwd_path  == -1)
	{
		error_msg(NULL, cmd, NULL, "Command not found");
			return (NULL);
	}
	return (add_path);
}

/*
** If command starts with a ~/: (ex: ~/../../bin/ls or ~/42/minishell)
** Duplicate the command but starting from it's index 1 (not 0 which is the
** tilde) and then I add "../.." to the beginning of the command.
**
** Ex: ~/../../bin/ls
** cmd_no_tilde = /../../bin/ls
** add_path_to_cmd = ../../../../bin/ls
**
**
** If command is an absoulte path: (ex: /bin/ls or ./minishell)
**		Duplicate command since it is already an absolute path.
**
** Obs.: execve will handle if the absolute path executable given dos't exist)
*/
char	*absolute_path(char *cmd)
{
	char	*cmd_no_tilde;
	char	*add_path_to_cmd;
	int		i;

	i = -1;
	if (ft_strncmp(cmd, "~/", 2) == 0)
	{
		if (!(cmd_no_tilde = ft_strdup(&cmd[1])))
			return (NULL);
		if (!(add_path_to_cmd = ft_strjoin("../..", cmd_no_tilde)))// or /home/user42??
			return (NULL);
		ft_strdel(&cmd_no_tilde);
	}
	else
	{
		if (!(add_path_to_cmd = ft_strdup(cmd)))
			return (NULL);
	}
	return (add_path_to_cmd);
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
char	*path_to_executable(t_list **env, t_command *cmd)
{
	char	*abs_path;
	char	*pwd_path;
	char	*env_path;
	char	**each_path_dir;

	if (!cmd->command)
		return (NULL);
	if (!(abs_path = ft_strdup("")))
		return (NULL);
	pwd_path = find_env_value(env, "PWD");
	env_path = find_env_value(env, "PATH");
	if (!(each_path_dir = ft_split_jb(env_path, ':')))
		return (NULL);
	if (cmd->command[0][0] != '/'
			&& ft_strncmp(cmd->command[0], "./", 2) != 0
			&& ft_strncmp(cmd->command[0], "../", 3) != 0
			&& ft_strncmp(cmd->command[0], "~/", 2) != 0
			&& env_path != NULL)
		abs_path = relative_path(cmd, each_path_dir, pwd_path);
	else
		abs_path = absolute_path(cmd->command[0]);
	ft_freetab(each_path_dir);
	return (abs_path);
}
