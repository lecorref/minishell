/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:34:20 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/22 21:35:43 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg_bash(t_list **cmd, int errnb, char *err_msg)
{
//	printf("ENTERED ERROR_MSG_BASH\n");
	ft_putstr_fd("bash: ", CMD_FD(*cmd)[2]);
	ft_putstr_fd(CMD(*cmd)[0], CMD_FD(*cmd)[2]);
	ft_putstr_fd(": ", CMD_FD(*cmd)[2]);
	if (errnb >= 0)
		ft_putstr_fd(err_msg, CMD_FD(*cmd)[2]);
	else
	{
		ft_putstr_fd(strerror(errno), CMD_FD(*cmd)[2]);
		ft_putchar_fd('\n', CMD_FD(*cmd)[2]);
	}
//	ft_free(cmd);  or free_command_list(t_list *cmd);
}

void	error_msg(t_list **cmd, int errnb, char *err_msg)
{
//	printf("ENTERED ERROR_MSG\n");
	ft_putstr_fd(CMD(*cmd)[0], CMD_FD(*cmd)[2]);
	ft_putstr_fd(": ", CMD_FD(*cmd)[2]);
	if (errnb >= 0)
		ft_putstr_fd(err_msg, CMD_FD(*cmd)[2]);
	else
	{
		ft_putstr_fd(strerror(errno), CMD_FD(*cmd)[2]);
		ft_putchar_fd('\n', CMD_FD(*cmd)[2]);
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
