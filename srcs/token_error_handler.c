#include "minishell.h"

int				tokenize_error_pipe(t_list **head, char **pipeline,
									int i, int fd_tmp)
{
	if (fd_tmp != -1)
		close(fd_tmp);
	while (pipeline[i])
		free(pipeline[i++]);
	free(pipeline);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	return (RT_FAIL);
}

void			*tokenize_error_sc(t_list **head, char **array)
{
	ft_freetab(array);
	if (head)
		ft_lstclear(head, &clear_commandlist);
	return (NULL);
}

int			p_t_e(char **str, int i, int *err)
{
	char	*skiped;

	*err = UTOKEN_OR;
	if (str[i + 1])
	{
		if (*str[i + 1] == '|')
			*err = UTOKEN_P;
		skiped = skip_char(str[i + 1], ' ');
		if (*skiped == '|')
			*err = UTOKEN_P;
		else if (!*skiped)
			*err = UTOKEN_ML;
	}
	return (*err);
}

int			parse_token_error(char **str, int i, int *err)
{
	char	*skiped;
	int		index;

	if (*str[i] == '|' && i > 0)
		return ((*err = p_t_e(str, i, err)));
	skiped = skip_char(str[i], ' ');
	if (*skiped == '|')
		return ((*err = UTOKEN_P));
	if (!str[i + 1])
	{
		index = ft_strlen(str[i]) - 1;
		if (str[i][index] == '|')
			return ((*err = UTOKEN_ML));
	}
	return (0);
}
