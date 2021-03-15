/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable_external_path_3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:16:50 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/15 02:08:57 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		cmd_found_more_times(t_command *cmd, char **split_path)
{
	int		i;

	i = -1;
	ft_putstr_fd("Command '", cmd->fd[2]);
	ft_putstr_fd(cmd->command[0], cmd->fd[2]);
	ft_putstr_fd("' is available in the following places\n", cmd->fd[2]);
	while (split_path[++i])
	{
		if (test_cmd(split_path[i], cmd->command[0]) == 0)
		{
			if (ft_strcmp(split_path[i], "/bin") == 0)
			{
				ft_putstr_fd(" * ", cmd->fd[2]);
				ft_putstr_fd(split_path[i], cmd->fd[2]);
				ft_putchar_fd('/', cmd->fd[2]);
				ft_putstr_fd(cmd->command[0], cmd->fd[2]);
				ft_putchar_fd('\n', cmd->fd[2]);
			}
		}
	}
}
