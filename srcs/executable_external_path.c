/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external_path.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/11 02:55:59 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
** It opens the directories passed as absolute paths to the argum env_path.
** Then it reads this directory and returns a pointer to a structure called
** dirent. One of the variables inside this struc is d_name which contains
** the filename.
** The filename(s) inside the directory is compared with the executable input
** by the user, and if the comparison is equal to 0, then this executable
** indeed exists inside this directory (return 0). Otherwise, there is not
** executable with the given name inside this directoy (return -1).
**
** st_mode: This field contains the file type and mode.
**				 > See inode(7) for further information.
** mode_t        st_mode;     File type and mode
** S_IXUSR       00100        owner has execute permission
*/
int			test_cmd(char *env_path, char *executable)
{
	int				cmp;
	DIR				*dp;
	struct dirent	*dirp;

	if (!(dp = opendir(env_path)))
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
** The readin of $PATH is done from the index last found to right.
** To make it work starting from the next index after the last one found, we
** would have to user a global variable. I don't think we have to go that deep
** since the correction sheet asks only to check if we are checking the $PATH
** from left to right (not from where it stopped on the last command, to right).
*/
char	*test_path_left_right(t_command *cmd, char *saved_path)
{
	char	**split_path;
//	char	**found_cmd;
	int		ret_test;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = 0;
	k = -1;
	ret_test = 1;
	if (!(split_path = ft_split_jb(saved_path, ':')))
		return (NULL);
//	if (!(found_cmd = (char**)malloc(sizeof(char*) + 3)))
//		return (NULL);
	while (split_path[++i])
	{
		if ((ret_test = test_cmd(split_path[i], cmd->command[0])) == 0)
		{
			j = j + 1;
			if (j == 1)
			{
				printf("Command '%s' is available in ", cmd->command[0]);
				printf("'%s/%s'\n", split_path[i], cmd->command[0]);
				printf("The command could not be located because '%s' ", split_path[i]);
				printf("is not included in the PATH environment variable.\n");
			}
			else if (j > 1)
			{
				printf("Command '%s' ", cmd->command[0]);
				printf("is available in the following places\n");
		//		while (split_path[++k])
		//		{
				if (ft_strcmp(split_path[i], "/bin") == 0)
				{
					j = i;
					printf(" * %s/%s\n", split_path[i], cmd->command[0]);
					//here I'll have the index for /bin (because its the most important path to keep
				}
				else if (i != j)
					printf(" * %s/%s\n", split_path[i], cmd->command[0]);
		//		}
				i = -1;
				printf("The command could not be located because ");
			//	while (split_path[++k])
			//	{
				printf("\'%s", split_path[i]);
				if (split_path[i + 1])
					printf(":");
				if (!split_path[i + 1])
					printf("\'");
				if (i == ft_count_tab(split_path))
					printf(" is not included in the PATH environment variable.\n");
			//	}
			//	k = -1;
			//	while (split_path[++k])
			//	{
				if (ft_strcmp(split_path[i], "/sbin") == 0)
				{
					printf("This is most likely caused by the lack of ");
					printf("administrative privileges associated with your ");
					printf("user account.\n");
					break ;
				}
		//		}
			}
		//	else
		//		break;
		}
		//	found_cmd[++j] = ft_strdup(split_path[i]);
	}
/*	found_cmd[j] = NULL;
	if (ret_test == -1 && found_cmd[0] && !found_cmd[1])
	{
		printf("Command '%s' is available in ", cmd->command[0]);
		printf("'%s/%s'\n", found_cmd[0], cmd->command[0]);
		printf("The command could not be located because '%s' ", found_cmd[0]);
		printf("is not included in the PATH environment variable.\n");
	}
	else if (ret_test == -1 && found_cmd[1])
	{
		i = -1;
		printf("Command '%s' ", cmd->command[0]);
		printf("is available in the following places\n");
		while (found_cmd[++i])
		{
			if (ft_strcmp(found_cmd[i], "/bin") == 0)
			{
				j = i;
				printf(" * /%s/%s\n", found_cmd[i], cmd->command[0]);
				//here I'll have the index for /bin (because its the most important path to keep
			}
			if (i != j)
				printf(" * /%s/%s\n", found_cmd[i], cmd->command[0]);
		}
		i = -1;
		printf("The command could not be located because ");
		while (found_cmd[++i])
		{
			printf("'/sbin");
			if (found_cmd[i + 1])
				printf(":");
			if (!found_cmd[i + 1])
				printf("\'");
			if (i == ft_count_tab(found_cmd))
				printf(" is not included in the PATH environment variable.\n");
		}
		i = -1;
		while (found_cmd[++i])
		{
			if (ft_strcmp(found_cmd[i], "/sbin") == 0)
			{
				printf("This is most likely caused by the lack of ");
				printf("administrative privileges associated with your ");
				printf("user account.\n");
				break ;
			}
		}
	}*/
//	ft_freetab(found_cmd);
	ft_freetab(split_path);
	return (NULL);
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
char	*relative_path(t_command *cmd, char **split_path, char *path,
															char *saved_path)
{
	char	*add_path;
	int		ret_env_path;
	int		i;

	i = -1;
	ret_env_path = -1;
	add_path = NULL;
	while (split_path[++i])
	{
		printf("\n%s\n", split_path[i]);
		if ((ret_env_path = test_cmd(split_path[i], cmd->command[0])) == 0)
		{
			add_path = add_path_to_cmd(split_path[i], cmd->command[0]);
			break ;
		}
	}
	if (ret_env_path == -1)
	{
		if (path && saved_path && ft_strcmp(path, saved_path) != 0)
			test_path_left_right(cmd, saved_path);
		error_msg(NULL, cmd, NULL, "command not found");
		return ("");
	}
	return (add_path);
}

/*
** If command starts with a ~/: (ex: ~/../../bin/ls or ~/42/minishell)
** Duplicate the command but starting from it's index 1 (not 0 which is the
** tilde) and then I add "../.." to the beginning of the command.
**
** Ex: ~/../../bin/ls
** add_path_to_cmd = /home/user42/../../bin/ls
**
**
** If cmd is an absoulte path: (ex: /bin/ls, ./minishell, /bin/lo, ../sbin/ip)
**		Duplicate command since it is already an absolute path.
**
** Obs.: execve will handle if the absolute path executable given does't exist)
*/
char	*absolute_path(t_command *cmd, char *home_path)
{
	char		*add_path_to_cmd;
	struct stat	statbuf;
	int			i;

	i = -1;
	add_path_to_cmd = NULL;
	if (stat(cmd->command[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		error_msg("bash", cmd, NULL, strerror(21));
		return ("");
	}
	if (ft_strncmp(cmd->command[0], "~/", 2) == 0)
	{
		if (!(add_path_to_cmd = ft_strjoin(home_path, (cmd->command[0] + 1))))
			return (NULL);
	}
	else
	{
		if (!(add_path_to_cmd = ft_strdup(cmd->command[0])))
			return (NULL);
	}
	return (add_path_to_cmd);
}
