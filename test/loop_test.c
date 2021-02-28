#include "minishell.h"

void		handle_sigint(int sign)
{
	if (sign == SIGINT)
		ft_putstr("\nminishell-1.0$ ");
}

void		print_array2(char **array)
{
        int                     i;

        i = -1;
		//printf(LINE(PRINT_LIST));
        while (array[++i])
                printf("array[%d] :|%s|\n", i, array[i]);
		printf("array[%d] :|%s|\n", i, array[i]);
        printf(LINE2);
}

void		print_fd(int *fd)
{
	printf("fd[0] = %d\tfd[1] = %d\tfd[2] = %d\n", fd[0], fd[1], fd[2]);
	printf(LINE2);
}

void		print_list(t_list *cmd)
{
	while (cmd)
	{
		print_array2(((t_command*)(cmd->content))->command);
		print_fd(((t_command*)(cmd->content))->fd);
		cmd = cmd->next;
		printf(LINE2);
	}
}

/*
** if the char is 'n' instead of '\n' :
** if I type '\n', it writes a newline. But if I type 'n', many times
** and continue after, it does nothing UNTIL I type '\n'.
** that's mean that the 'enter' key sends a signal to read() saying :
** "now you read all that has been wrote to the stdin !". Like when a fifo
** write end fd is closed (which says to read end that it can read the buffer)
**
** We could say that keyboard & read(fd0) are linked by a fifo buffer (pipe)
** & that enter key closes the write end fd.
** All characters typed after n aren't displayed bc my gnl puts a
** 0 at the place of flag character.
**
** If I type '\n' before typing 'n', it just writes '\n', BUT, we can't backspace
** it. That means that read() has read. But since 'n' hasn't been met, buffer
** is still full and gnl didn't returned
** So, when it's a '\n' character' inside gnl, it sends signal to read() to read
** plus returning gnl has he met '\n'
** 
** ctrl^D sends EOT signal where read() then returns 0. So it says to read()
** to read 0 Bytes, so that read() returns 0.
*/

int		check_status(char **line, char *buf)
{
	if (**line)
		return (1);
	else
		return (0);
	(void)buf;
}

/*
**		(ctrl^D)-,       ,-(ctrl^C)
**               v       v
** > "hello      \0"world  
** > Bla
**
** My current behavior----->line--->|helloBla|
** what we should have----->line--->|Bla|
**
** When ctrl^C is sent, buffer is empty. Why ?
*/

int		gnl_hacked(int fd, char **line)
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
		printf("a");
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
			if (!check_status(line, buf[fd]))
				return (0);
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	printf("\nb\n");
	return (1);
}

int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	t_list	*cmd;
	t_list	*cmd_cp;
	t_list	*env;
	int		ret;
	char	**arr_env;

	signal(SIGINT, handle_sigint);
	env = create_env_list(ep);
	arr_env = env_list_to_tab(env);
	ft_putstr("minishell-1.0$ ");
	//while ((ret = get_next_line(0, &line)) > 0)
	while ((ret = gnl_hacked(0, &line)) > 0)
	{
		printf("this shit has to be parsed: %s\n", line);
		//printf("BEFORE TOK\n");
		cmd = tokenize_line_jb(line, &env);
		//printf("\nAFTER TOK\n");
		free(line);
		//print_list(cmd);
		cmd_cp = cmd;
		while (cmd_cp)
		{
		/*	printf(LINE(PRINT_LINK));
			print_array2(((t_command*)(cmd_cp->content))->command);
			print_fd(((t_command*)(cmd_cp->content))->fd);*/
			errno = 0;
			exec_t(&env, (t_command*)(cmd_cp->content), arr_env);
			//execute_command(&env, (t_command*)(cmd_cp->content));
			cmd_cp = cmd_cp->next;
			/*printf(LINE2);
			printf(LINE2);
			printf(LINE2);*/
		}
		//print_list(cmd);
		ft_lstclear(&cmd, &clear_commandlist);
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	ft_lstclear(&env, &clear_envlist);
	ft_freetab(arr_env);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		ft_putstr("exit\n");
	//system("leaks testit");
	if (ac && av && ep)
		;
	return (0);
}
