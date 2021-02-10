/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:34:20 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/10 02:40:15 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//#include "../includes/minishell.h"

void	error_message(t_command *cmd, int writes_bash)
{
	if (writes_bash == 1)
	{
		ft_putstr_fd("bash: ", cmd->fd[2]);
		ft_putstr_fd(cmd->command[0], cmd->fd[2]);
		ft_putstr_fd(": ", cmd->fd[2]);
		ft_putstr_fd(strerror(errno), cmd->fd[2]);// or error_message if errno does not gives the correct message????
		ft_putchar_fd('\n', cmd->fd[2]);
	//	ft_free(cmd);  or free_command_list(t_list *cmd);
//		exit(errno);// I think I will put it in the function
	}
	else if (writes_bash == 0)
	{
		ft_putstr_fd(cmd->command[0], cmd->fd[2]);
		ft_putstr_fd(": ", cmd->fd[2]);
		ft_putstr_fd(strerror(errno), cmd->fd[2]);//or error_message?????
		ft_putchar_fd('\n', cmd->fd[2]);
	//	ft_free(cmd);  or free_command_list(t_list *cmd);
//		exit(errno);// I think I will put it in the function
	}
	exit(errno);
}

void	exit_manager(char **cmd, int execve_ret)
{
	(void)execve_ret;
	if (!cmd[0][0])
//	{
//		signal_exit();
		free(cmd);
//		exit(0);
//	}
}
