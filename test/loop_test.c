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
        printf(LINE(PRINT_ARRAY2));
        while (array[++i])
                printf("array[%d] :|%s|\n", i, array[i]);
        printf(LINE2);
}

void		print_fd(int *fd)
{
	printf("fd[0] = %d\tfd[1] = %d\tfd[2] = %d\n", fd[0], fd[1], fd[2]);
}

void		print_list(t_list *cmd)
{
	printf(LINE(PRINT_LIST));
	while (cmd)
	{
		print_array2(((t_command*)(cmd->content))->command);
		print_fd(((t_command*)(cmd->content))->fd);
		cmd = cmd->next;
		printf(LINE2);
	}
}


void		clear_commandlist(void *content)
{
	int		i;
	int		*fdcp;
	char	**args;

	fdcp = ((t_command*)content)->fd;
	args = ((t_command*)content)->command;
	free(fdcp);
	i = -1;
	while (args[++i])
		free(args[i]);
	free(args);
}


int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	t_list	*cmd;
	t_list	*env;
	int		ret;

	signal(SIGINT, handle_sigint);
	env = create_env_list(ep);
	ft_putstr("minishell-1.0$ ");
	while ((ret = get_next_line(0, &line)) > 0)
	{
		//printf("this shit has to be parsed: %s\n", line);
		printf("BEFORE TOK\n");
		cmd = tokenize_line_jb(line, &env);
		printf("\nAFTER TOK\n");
		print_list(cmd);
		//execute_commands(head, ep);
		ft_lstclear(&cmd, &clear_commandlist);
		free(line);
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		ft_putstr("exit\n");
	//system("leaks testit");
	if (ac && av && ep)
		;
	return (0);
}
