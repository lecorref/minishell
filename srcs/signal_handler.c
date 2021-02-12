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
	//	errno = 131; //need that????
	//	need to free any variable? maybe environment variables?
	}
}

void	ctrl_c_handler(int signal)
{
	if (signal == SIGINT)
		errno = 1;
}

void	ctrl_d_handler(char *line)//char *line)
{
	line = NULL;
//	free(line);// need to free even though there is no line??
	ft_putstr_fd("exit\n", 2);
	exit(0);//after a ctrlD on an empry prompt, the $? == 0

}
