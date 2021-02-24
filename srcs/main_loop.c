/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 21:13:42 by jfreitas          #+#    #+#             */

/*   Updated: 2021/02/23 20:57:56 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	ope(t_list **env, t_command *cmd, void (func)(t_list *, t_command *))
//{
//	(func)(env, cmd);
//} ???? I DON'T KNOW HOW TO DO IT WITH POINTER TO FUNCITONS :/ HELLLLP
void	execute_command(t_list **env, t_command *cmd)
{
	if (ft_strcmp(cmd->command[0], "echo") == 0)
		echo_builtin(cmd);
	else if (ft_strcmp(cmd->command[0], "pwd") == 0)
		pwd_builtin(cmd);
	else if (ft_strcmp(cmd->command[0], "exit") == 0)
		exit_builtin(cmd);
	else if (ft_strcmp(cmd->command[0], "cd") == 0)
		cd_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "export") == 0)
		export_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "unset") == 0)
		unset_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		env_builtin(env, cmd);

	//else if cmd == $ ??
	else
		executable_builtin(env, cmd);
}

/*
** Printing the propt by searching for the SESSION_MANAGER environmetal
** variable that is inside of the char **ep argument of the main.
** Then spliting the line by / and : just to get the exact name to then print
** it on the stdout (fd1).
*/
int		prompt(t_list *env)
{
	char	*find_env;
	char	**line;
	char	**session_manager;

	if ((find_env = find_env_value(&env, "SESSION_MANAGER")) == NULL)
	{
		ft_putstr_fd("minishell$ ", 1);
		return (0);
	}
	else
	{
		find_env = ft_strchr(find_env, '/');
		line = ft_split(find_env, '/');
		session_manager = ft_split(line[0], ':');
		ft_putstr_fd("\033[1;32m", 1);
		ft_putstr_fd(session_manager[0], 1);
		ft_putstr_fd("% ", 1);
		ft_putstr_fd("\033[0m", 1);
		free(line);
		free(session_manager);
		return (0);
	}
}

int		gnl_ctrld(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
		{
			if (**line)
				buf[fd][0] = 0;
			else
				return (0);
		}
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}

/* Signal
**
** If the disposition is set to SIG_DFL, then the default action associated
** with the signal (see signal(7)) occurs.
**
**
** ctrl-C
** As ctrlC will just print ^C and go to the new line AT ANYTIME, we can handle
** it inside the loop for when:
** 1. a blocking command is typed, for example: grep h
** 2. if something is typed and ctrlD is typed before the return button, it
** writes ^C and goes to new line/promt.
** 3. for when command line is empty and no return button pressed (crtlC is
** pressed instead).
**
** ctrl-\
** Before the user input something, if ctrl\ is typed, nothing happpens inside
** the loop, if ctrl\ is typed, it has a different comportament).
** 1. if a blocking command is typed, ctrl\ kils the comman with a (core dumped)
** message and goes to a new line - i.e.: shows the prompt).
** 2. if something is typed and ctrl\ is typer before the return button, it
** does nothing.
*/

/* ctrl-D
** Before the user input something, if ctrlD is typed, the string "exit" is
** written after the prompt, then the shell closes, and $SHLVL environment
** variable is decreased after any input, if ctrlD is typed, it has a different
** comportament:
** 1. if a blocking command is typed, ctrlD stops the command and goes to a
** new line - i.e.: shows the prompt).
** 2. if something is typed and ctrlD is typed before the return button, it
** does nothing.
*/

/* This loop should only exit on ctrlD, sigquit and "exit" builtin.
** It should be able to refuse to exit on Ctrl-d if needed (when something is
** typed and then the ctrlD is typed before the return button, it doesn't exit;
** otherwise it'll exit, either the command or the shell).
** gnl return:
** 1 = line read,
** 0 = EOF
** -1 = error
*/
int		main_loop(t_list *env)
{
	t_list	*cmd;
	t_list	*cmd_cp;
	char	*line;
	char	*save_line;
	int		ret_gnl;

	save_line = ft_strdup("");
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, ctrl_back_slash_handler);
	prompt(env);
	while ((ret_gnl = gnl_ctrld(0, &line)) == 1)
	{
		if (!line[0])
		{
			free(line);
			prompt(env);
			continue ;
		}
		cmd = tokenize_line_jb(line, &env);
		// inside this tokenize_line function -> to do:
		// 1.split it by | or ; or > or < or >>  and save it to the
		// t_cmd cmd->command (multiples of the same redirection has to fail) -
		// try to mix pipes and redirections 
		// 2.0. handle single quotes - try single quotes at arguments of
		// functions (also quoted empty args '', or whitespace, or ';' or and
		// also environment variables)
		// 2.1. and double quotes (and weird use of \ for double quotes, and
		// enrironment variabes - NO need to try multilines)
		// 3. handle $ENV_VAR (also with double quotes ex: echo "$USER")
		// 4. handle $? and send it to a function
		cmd_cp = cmd;
		while (cmd_cp)
		{
			execute_command(&env, (t_command*)(cmd_cp->content));
			cmd_cp = cmd_cp->next;
		}
		prompt(env);
		ft_lstclear(&cmd, &clear_commandlist);// same as void free_command_list(t_list **command)???
		// ft_lstdel(&env, free_env); -> at the very end of everything??? or here insede this loop????
		free(line);
		line = NULL;
	}
	/////////////////////JOY WORKING ON CTRLD AFTER CTRLC
	if (ret_gnl == 0)
	{
		ctrl_d_handler(line);
		main_loop(env);
	}
	/////////////////////JOY WORKING ON CTRLD AFTER CTRLC

	free(line);
	line = NULL;
	ft_lstclear(&env, &clear_envlist);
	if (ret_gnl == -1)//??do that all errors should have been already handled before it gets to this part
		return (-1);
	return (0);
}

