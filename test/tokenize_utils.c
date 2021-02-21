#include "minishell.h"

void		delete_remaining_char(char *str, char c)
{
	int			end_index;

	end_index = ft_strlen(str) - 1;
	if (str[end_index] == c)
		str[end_index] = 0;
}

int			*init_fd()
{
	int		*fd;

	if (!(fd = (int*)malloc(sizeof(int) * 3)))
		return (NULL);
	fd[0] = 0;
	fd[1] = 1;
	fd[2] = 2;
	return (fd);
}

char		*skip_char(char *str, char c)
{
	while (*str)
		if (*str++ != c)
			return (str - 1);
	return (str);
}

int			is_symbol(int c)
{
	return (c == '>' || c == '<' || c == ' ' || c == '\'' || c == '\"');
}

int			is_symbol_v2(int c)
{
	return (c == '>' || c == '<' || c == ' ');
}

char		*end_of_object(char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '>' || *str == '<')
			return (str);
		str++;
	}
	return (str);
}
