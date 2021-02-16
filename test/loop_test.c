#include "minishell.h"

void		handle_sigint(int sign)
{
	if (sign == SIGINT)
		ft_putstr("\nminishell-1.0$ ");
}

int			main(int ac, char *av[], char *ep[])
{
	char	*line;
	t_list	*head;
	int		ret;

	signal(SIGINT, handle_sigint);
	ft_putstr("minishell-1.0$ ");
	while ((ret = get_next_line_jb(0, &line)) > 0)
	{
		//printf("this shit has to be parsed: %s\n", line);
		printf("\nBEFORE TOK\n");
		head = tokenize_line_jb(line);
		printf("\nAFTER TOK\n");
		//execute_commands(head, ep);
		//ft_lstclear(&head, &clear);
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
