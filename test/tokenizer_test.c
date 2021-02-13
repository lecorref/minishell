#include "minishell.h"

# define CMD_FD(NAME) ((t_command *)((NAME)->content))->fd
# define CMD_ARGS(NAME) ((t_command *)((NAME)->content))->command

int main() {
	t_list *list;

	list = NULL;
	tokenize_line("ahah > test; balbla 2> youpi | test -c >> truc", &list);
	while (list)
	{
		printf("first cmd arg is: %s, and the fds are: %d, %d and %d\n",
				CMD_ARGS(list)[0], CMD_FD(list)[0], CMD_FD(list)[1], CMD_FD(list)[2]);
		list = list->next;
	}
	return (0);
}
