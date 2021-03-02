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

void		display_prompt(int sign)
{
	if (sign == SIGINT)
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
}

void		set_line_eraser(int sign)
{
	if (sign == SIGINT)
	{
		g_line_eraser = 1;
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
	}
}
