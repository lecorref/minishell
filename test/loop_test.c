#include "minishell.h"

void		handle_sigint(int sign)
{
	if (sign == SIGINT)
		ft_putstr("\nminishell-1.0$ ");
}

int			main()
{
	char	*line;
	int		ret;

	signal(SIGINT, handle_sigint);
	ft_putstr("minishell-1.0$ ");
	while ((ret = get_next_line_jb(0, &line)) > 0)
	{
		printf("this shit has to be parsed: %s\n", line);
		//parser(line, ep);
		free(line);
		ft_putstr("minishell-1.0$ ");
	}
	free(line);
	if (ret == -1)
		return (-1);
	if (ret == 0)
		ft_putstr("exit\n");
	//system("leaks testit");
	return (0);
}
