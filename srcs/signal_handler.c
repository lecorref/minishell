/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 19:18:04 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/09 22:00:31 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
//#include "../includes/minishell.h"

// void	signal(int signum, int handler);
void	ctrl_back_slash_handler(int signal)
{
	(void)signal;
	ft_putstr_fd("\b\b  \b\b", 2);// \b means backspace for printf
}

void	ctrl_d_handler(int fd)
{
//	(void)signal;
	ft_putstr_fd("exit\n", fd);
	exit(0);
	//OBS: IT'S NOT EXITING ON THE fIRST CRTLD .. ONLY ON THE SECOND ONE
}

//void	ctrl_c_handler(int signal)
//{
//	(void)signal;
//	ft_putstr_fd("exit\n", cmd->fd[2]);
//	exit(0);
	//OBS: IT'S NOT EXITING ON THE IRST CRTLD .. ONLY ON THE SECOND ONE
//}

//void	exit_handler(int signal, t_command *cmd)???
//{
//	(void)signal;
//	ft_putstr_fd("exit\n", cmd->fd[2]);
//	exit(0);
	//OBS: IT'S NOT EXITING ON THE IRST CRTLD .. ONLY ON THE SECOND ONE
//}
