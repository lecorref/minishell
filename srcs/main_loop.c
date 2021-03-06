#include "minishell.h"

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
**
** ctrl^D sends EOT signal where read() then returns 0. So it says to read()
** not to read 0 Bytes but to read the fifo buffer then returns 0.
**
**
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

void	eraser_checker(char *line)
{
	errno = 0;//later delete either errno or g_exit_status
	g_exit_status = 0;
	if (g_line_eraser == 1)
	{
		ft_memset(line, 0, ft_strlen(line));
		g_line_eraser = 0;
	}
}

	// Now here we also check if g_line_eraser is 1 with the eraser_checker, bc
	// if we type "lsCTRL-C" it WILL enter the if(!(read...) and then it'll
	// enter here, so we need to erase line since we have typed ctrlC, for
	// But if we have typed "lsCTRL-D" instead, then the ctrlC signal would not
	// have been called, therefore g_line_eraser would be 0, Therefore
	// eraser_checker would not delete the line, and so we would enter on the
	// first if statement here.
int		check_ctrld(char **line)
{
	eraser_checker(*line);
	//	signal(SIGINT, set_line_eraser);// dont need to use it anymore
	if (**line && g_line_eraser == 0)
	{
		errno = 0;//later delete either errno or g_exit_status 
		g_exit_status = 0;
		return (1);
	}
	else if (!**line)
	{
		ft_putstr_fd("exit\n", 2);
		return (0);
	}
	return (1);
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
	// now here if ctrl is typed, g_line_eraser is set to 1 inside the 
	// display_prompt() fucntion, because it can enter the while loop down
	// below and still be waiting for a ctrlC signal. The first ctrlD will not
	// enter on the if (!(read...), so if we type "lsCTRL-DCTRL-C", the
	// eraser_checker will check if g_line_eraser is 1 (before it was still
	// 0 since inside the first signal handler call, g_line_eraser was nothing
	// being set to 1, but now it's).
	// So now eraser_checker WILL ideed memset the line since we have typed
	// "lsCTRL-DCTRL-C" and as ctrlC was typed, we have to "erase" line.
	// btw this signal will be active and waiting for any signal untill this 
	// very functions returns I suppose, so it will be active and waiting for
	// the signal inside the check_ctrld() function too, that is why we dont
	// need to call signal() there again.

int		gnl_ctrld(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	signal(SIGINT, ctrl_c_handler);
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

int		execute_command(t_list **env, t_command *cmd)
{
	int	ret;

	print_cmd(cmd);//TEST - TO DELETE LATER
	if ((ret = is_builtin(cmd)))
		ret = execute_builtin(env, cmd, ret);
	else
		ret = execute_extern(env, cmd);
	close_fd(cmd->fd);
	return (ret);
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

void	print_tok(void *content)
{
	printf("unexpanded : |%s|\t\tfd0: %d\tfd1: %d\n",
			((t_command*)content)->unexpanded,
			((t_command*)content)->fd[0],
			((t_command*)content)->fd[1]);
}

int		executer(t_list **env, t_list *cmd)
{
	while (cmd)
	{
		expander(env, COMMAND(cmd));
		if ((g_exit_status = execute_command(env, COMMAND(cmd))) == -2)
			return (g_exit_status);
		cmd = cmd->next;
	}
	return (g_exit_status);
}

int		main_loop(t_list **env)
{
	t_list	*cmd;
	char	*line;
	int		ret_gnl;

	signal(SIGQUIT, ctrl_back_slash_handler);
	ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
	while ((ret_gnl = gnl_ctrld(0, &line)) > 0)
	{
		cmd = tokenizer(line);
		free(line);
		ft_lstiter(cmd, &print_tok);
		executer(env, cmd);
		ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
		ft_lstclear(&cmd, &clear_commandlist);
	}
	free(line);
	ft_lstclear(env, &clear_envlist);
	if (ret_gnl == -1)
		return (-1);
	return (0);
}
