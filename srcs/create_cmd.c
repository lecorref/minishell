#include "minishell.h"

void add_cmd_to_list(char *cmd, t_list **head, int *fd)
{
	t_command	*new_cmd;
	t_list		*link;
	t_list		*tmp;

	new_cmd = malloc(sizeof(t_command));
	new_cmd->fd = fd;
	new_cmd->command = ft_strsplit(cmd, ' ');
	link = (t_list *)malloc(sizeof(t_list));
	link->content = new_cmd;
	link->next = NULL;
	tmp = *head;
	if (!tmp)
		*head = link;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = link;
	}
}
