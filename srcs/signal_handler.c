#include "minishell.h"

void	ctrl_back_slash_handler(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 2);// \b = backspace
}

void	ctrl_back_slash_handler_quit(int signal)
{
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		errno = 131; //need that????
	//	need to free any variable? maybe environment variables?
	}
}

void	ctrl_c_handler(int signal)
{
	if (signal == SIGINT)
	{
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
		errno = 130;// or 1???
	}
}

void	ctrl_d_handler(char *line)
{
	char	*save_line;

	save_line = ft_strdup("");
	if (!line[0])
	{
		ft_putstr_fd("exit\n", 2);
		exit(0);//after a ctrlD on an empry prompt, the $? == 0
	}
	if (line[0])
	{
		line = ft_strjoin(save_line, line);
		free(save_line);
	}
}
