#include "minishell.h"

/*
** When ctrl\ is pressed on an empty line it does nothing.
** \b = backspace
** So here after a ctrl\ is pressed, the cursos moves backwards 2 times, to
** erase ^\ the characters, then it prints 2 spaces to fillout the spaces these
** ^\ characters where taking, and last, it goes back again 2 times to put the
** cursor in place.
*/
void	ctrl_back_slash_handler(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 2);
}

void	ctrl_back_slash_handler_quit(int signal)
{
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		errno = 131; //do it like that????
	//	need to free any variable? maybe environment variables?
	}
}

void	display_prompt(int sign)
{
	if (sign == SIGINT)
	{
		g_exit_status = 1;
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
		errno = 130;
	}
}

void	set_line_eraser(int sign)
{
	if (sign == SIGINT)
	{
		g_exit_status = 1;
		g_line_eraser = 1;
		ft_putstr_fd("\n\033[1;32mminishell$\033[0m ", 1);
	}
}
