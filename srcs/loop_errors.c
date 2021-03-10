#include "minishell.h"

int		return_to_main(t_list **env, char *line, int ret_gnl)
{
	free(line);
	ft_lstclear(env, &clear_envlist);
	if (ret_gnl == -1)
		return (RT_FAIL);
	return (RT_EXIT);
}

void	print_token_error(char *str)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
}

int		token_error_manager(int err)
{
	if (!err)
		return (1);
	if (err == UTOKEN_P)
		print_token_error(E_UTOKEN_P);
	else if (err == UTOKEN_SC)
		print_token_error(E_UTOKEN_SC);
	else if (err == UTOKEN_ML)
		print_token_error(E_UTOKEN_ML);
	else if (err == UTOKEN_OR)
		print_token_error(E_UTOKEN_OR);
	if (g_line_eraser == 0)
		ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
	g_exit_status = 2;
	return (0);
}

int		empty_line(char *line)
{
	char	*skiped;

	skiped = skip_char(line, ' ');
	if (!*skiped)
	{
		free(line);
		if (g_line_eraser == 0)
			ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
		return (1);
	}
	return (0);
}
