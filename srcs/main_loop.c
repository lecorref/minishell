/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 21:13:42 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/02 02:15:57 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	line_eraser;// GLOBAL VARIABLE MUST START WITH g_
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
	return (0);
}*/

void		display_prompt(int sign)
{
	if (sign == SIGINT)
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
}

void		set_line_eraser(int sign)
{
	if (sign == SIGINT)
	{
		line_eraser = 1;
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
	}
}

/*
** If the char is 'n' instead of '\n':
** if I type '\n', it writes a newline. But if I type 'n', many times
** and continue after, it does nothing UNTIL I type '\n'.
** that means that the 'enter' key sends a signal to read() saying:
** "now you read all that has been wrote to the stdin!". Like when a fifo
** write end fd is closed (which says to read end that it can read the buffer).
**
** We could say that keyboard & read(fd0) are linked by a fifo buffer (pipe)
** & that enter key closes the write end fd.
** All characters typed after n aren't displayed because my gnl puts a 0 at
** the place of flag character.
**
** If I type '\n' before typing 'n', it just writes '\n', BUT, we can't
** backspace it. That means that read() has read. But since 'n' hasn't been met,
** line is still full and gnl didn't returned.
** So, when it's a '\n' character inside gnl, it sends a signal to read() to
** read, plus returning gnl has the met '\n'.
**
** ctrl^D sends EOT signal where read() then returns 0. So it says to read()
** not to read 0 Bytes but to read the fifo buffer then returns 0.
*/

/*
** We enter into check_ctrld when read returns 0, so when ctrl^D or ctrl^C is
** typed.
**
** ctrl^D after input: line exists -read() has read and gnl joined buf to line.
** ctrl^D with empty input: exit read() & so the minishell.
** ctrl^C: line inputed before doesn't exists because read() hadn't read.
** If inputed ctrl^D had been hited before, a line exists.
**
** When ctrl^D is hited, 2 behaviors:
**				- If Nothing had already been typed, exit the loop (return 0).
**				- else we want to stay into the read() loop to continue to
**				  create the line : don't return.
**
** When ctrl^C is hited, 1 behavior :
**				- Display a new prompt, line erased.
**
** If ctrl^D has been typed, signal() into check_ctrld will take advantage.
** Therefore, the ctrl^C signal handler called into this func has to set global
** variable line_eraser to 1, to say to the loop to erase the line.
**
** When ctrl^C is typed after a ctrl^D this is signal() inside check_ctrld()
** which catch it although we don't enter again in this func(). The only moment
** when we go into it is when ctrl^D is hited.
** But when ctrl^D is hited & read() continues, this signal() takes effect and
** takes advantage and will act when ctrl^C will be hited, until we go out of
** gnl which would have returned 1, then the more general signal() goes back.
*/
int		check_ctrld(char **line)
{
	signal(SIGINT, set_line_eraser);
	if (**line && line_eraser == 0)
		return (1);
	else
	{
		ft_putstr_fd("exit\n", 2);
		return (0);
	}
}

void	eraser_checker(char *line)
{
	if (line_eraser == 1)
	{
		ft_memset(line, 0, ft_strlen(line));
		line_eraser = 0;
	}
}

/*
** 'ENTER':						   ~> fifo write end closed(?). read() can read
**									  the fifo buffer, then wait to read again.
** ctrl^D : read(fd0) -> returns 0 ~> EOT. fifo write end closed and read()
**									  returns 0, buffer looks like being
**									  fullfiled anyway, which means read() act.
** ctrl^C : no read() -> no return ~> SIGINT. As signal() is used, pgm doesn't
**									  interrupt & currently print a new prompt,
**									  but still interrupt read without any read
**									  of buffer. No read() at all from the fifo
**									  buffer. read() returns 0 anyway.
**									  So, ctr^C handler must erase the line
**									  and display a new prompt.
**
**
**			(ctrl^D)-,       ,-(ctrl^C)
**                   v       v
** > "hello          \0"world
** > Bla
**
** Old behavior       ----->line--->|helloBla|
** what we should have----->line--->|Bla|
**
** When ctrl^C is sent, line is empty because if it exists it has been erased.
*/
int		gnl_ctrld(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	signal(SIGINT, display_prompt);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
			if (!check_ctrld(line))
				return (0);
		eraser_checker(*line);
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

/* This loop should only exit on ctrlD (sigquit) and "exit" builtin.
** It should be able to refuse to exit on Ctrl-d if needed (when something is
** typed and then the ctrlD is typed before the return button, it doesn't exit;
** otherwise it'll exit, either the command or the shell).
** gnl return:
** 1 = line read,
** 0 = EOF
** -1 = error
*/
int		main_loop(t_list **env)
{
	t_list	*cmd;
	t_list	*cmd_cp;
	char	*line;
	int		ret_gnl;

	line_eraser = 0;
	signal(SIGQUIT, ctrl_back_slash_handler);
	ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
	while ((ret_gnl = gnl_ctrld(0, &line)) > 0)
	{
		/*if (!line[0])
		{
			free(line);
			ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
			continue ;
		}*/
		cmd = tokenize_line_jb(line, env);
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
			execute_command(env, (t_command*)(cmd_cp->content));
			cmd_cp = cmd_cp->next;
		}
		ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
		ft_lstclear(&cmd, &clear_commandlist);
		// same as void free_command_list(t_list **command)???
		// ft_lstdel(&env, free_env); -> at the very end of everything???
		// or here insede this loop????
		free(line);
	}
	free(line);
	ft_lstclear(env, &clear_envlist);
	if (ret_gnl == -1)//??do thati? all errors should have been already handled
					  //before it gets to this part?
		return (-1);
	return (0);
}

