#include "minishell.h"

void			link_lists(t_list **head, t_list *new)
{
	t_list		*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_list			*tokenizer(char *line, int *err)
{
	int			i;
	t_list		*head;
	t_list		*tmp;
	char		**execution_lines;
	char		*skiped;

	head = NULL;
	if (!(execution_lines = split_with_exception(line, ';', "\'\"")))
		return (NULL);
	i = -1;
	while (execution_lines[++i])
	{
		tmp = NULL;
		skiped = skip_char(execution_lines[i], ' ');
		if (*skiped == ';' && (*err = UTOKEN_SC))
			return (tokenize_error_sc(&head, execution_lines));
		if ((pipeline_n_link(&tmp, execution_lines[i], err)) == RT_FAIL)
			return (tokenize_error_sc(&head, execution_lines));
		link_lists(&head, tmp);
	}
	ft_freetab(execution_lines);
	return (head);
}
