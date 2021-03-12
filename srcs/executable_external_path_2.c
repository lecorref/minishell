/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external_path_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/12 17:24:32 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cmd_found_once(t_command *cmd, char **split_path, int j)
{
	int	i;

	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			j++;
			printf("Command '%s' is available in ", cmd->command[0]);
			printf("'%s/%s'\n", split_path[i], cmd->command[0]);
			printf("The command could not be located because '%s' ",
																split_path[i]);
			printf("is not included in the PATH environment variable.\n");
		}
	}
}

static void	cmd_found_more_times(t_command *cmd, char **split_path)
{
	int	i;

	i = -1;
	printf("Command '%s' ", cmd->command[0]);
	printf("is available in the following places\n");
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
			if (ft_strcmp(split_path[i], "/bin") == 0)
				printf(" * %s/%s\n", split_path[i], cmd->command[0]);
	}
	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
			if (ft_strcmp(split_path[i], "/bin") != 0)
				printf(" * %s/%s\n", split_path[i], cmd->command[0]);
	}
}

static void	cmd_found_more_times_2(t_command *cmd, char **split_path)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	printf("The command could not be located because '");
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			printf("%s", split_path[i]);
			j = i;
			while (split_path[j])
			{
				if (split_path[j + 1])
					if (test_cmd(split_path[j + 1], cmd->command[0]) == 0)
						printf(":");
				j++;
			}
		}
	}
	printf("' is not included in the PATH environment variable.\n");
}

static void	cmd_found_more_times_3(t_command *cmd, char **split_path)
{
	int i;

	i = -1;
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			if (ft_strcmp(split_path[i], "/sbin") == 0)
			{
				printf("This is most likely caused by the lack of ");
				printf("administrative privileges associated with your ");
				printf("user account.\n");
				break ;
			}
		}
	}
}

void	*test_path_left_right_2(t_command *cmd, char **split, int ret, int j)
{
	if (ret == -1 && j == 0)
	{
		error_msg("bash", cmd, NULL, strerror(2));
		ft_freetab(split);
		return (NULL);
	}
	if (j == 1)
		cmd_found_once(cmd, split, j);
	else if (j > 1)
	{
		cmd_found_more_times(cmd, split);
		cmd_found_more_times_2(cmd, split);
		cmd_found_more_times_3(cmd, split);
	}
	ft_freetab(split);
	return ("");
}
