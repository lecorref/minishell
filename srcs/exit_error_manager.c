/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:34:20 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/24 14:37:48 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg_bash(t_command *cmd, int errnb, char *err_msg)
{
//	printf("ENTERED ERROR_MSG_BASH\n");
	ft_putstr_fd("bash: ", cmd->fd[2]);
	ft_putstr_fd(cmd->command[0], cmd->fd[2]);
	ft_putstr_fd(": ", cmd->fd[2]);
	if (errnb >= 0)
		ft_putstr_fd(err_msg, cmd->fd[2]);
	else
	{
		ft_putstr_fd(strerror(errno), cmd->fd[2]);
		ft_putchar_fd('\n', cmd->fd[2]);
	}
//	ft_free(cmd);  or free_command_list(t_list *cmd);
}

void	error_msg(t_command *cmd, int errnb, char *err_msg)
{
//	printf("ENTERED ERROR_MSG\n");
	ft_putstr_fd(cmd->command[0], cmd->fd[2]);
	ft_putstr_fd(": ", cmd->fd[2]);
	if (errnb >= 0)
		ft_putstr_fd(err_msg, cmd->fd[2]);
	else
	{
		ft_putstr_fd(strerror(errno), cmd->fd[2]);
		ft_putchar_fd('\n', cmd->fd[2]);
	}
//	ft_free(cmd);  or free_command_list(t_list *cmd);
}

void	exit_manager(char **cmd, int execve_ret)
{
	(void)execve_ret;
	(void)cmd;
//	if (!cmd[0][0])
//	{
//		free(cmd);
//		exit(0);
//	}
}
