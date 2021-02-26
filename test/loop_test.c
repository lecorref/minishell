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

int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	t_list	*cmd;
	t_list	*cmd_cp;
	t_list	*env;
	int		ret;

	signal(SIGINT, handle_sigint);
	env = create_env_list(ep);
	ft_putstr("minishell-1.0$ ");
	while ((ret = get_next_line(0, &line)) > 0)
	{
		//printf("this shit has to be parsed: %s\n", line);
		//printf("BEFORE TOK\n");
		cmd = tokenize_line_jb(line, &env);
		//printf("\nAFTER TOK\n");
		free(line);
		//print_list(cmd);
		cmd_cp = cmd;
		while (cmd_cp)
		{
			printf(LINE(PRINT_LINK));
			print_array2(((t_command*)(cmd_cp->content))->command);
			print_fd(((t_command*)(cmd->content))->fd);
			execute_command(&env, (t_command*)(cmd_cp->content));
			cmd_cp = cmd_cp->next;
			printf(LINE2);
			printf(LINE2);
			printf(LINE2);
		}
		//print_list(cmd);
		ft_lstclear(&cmd, &clear_commandlist);
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	ft_lstclear(&env, &clear_envlist);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		ft_putstr("exit\n");
	//system("leaks testit");
	if (ac && av && ep)
		;
	return (0);
}
