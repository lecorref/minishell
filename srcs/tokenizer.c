#include "minishell.h"

static int	*init_fd(void)
{
	int		*fd;

	fd = malloc(sizeof(int) * 4);
	bzero(fd, sizeof(int) * 4);
	fd[0] = 0;
	fd[1] = 1;
	fd[2] = 2;
	return (fd);
}


static void	check_pipe(char *line, t_list **head)
{
	char	**commands;
	char	**ptr;
	int		*fd;
	int		pip[2];
	int		save;

	save = 0;
	commands = ft_strsplit(buff, '|');
	ptr = commands;
	while (*command != NULL)
	{
		fd = init_fd();
		pipe(pip);
		fd[0] = save;
		save = pip[0];
		if (*(COMMAND + 1) != NULL)
			fd[1] = pip[1];
		else
			close(pip[1]);
		check_redirect(*commands, head, fd);
		free(*commands);
		commands++;
	}
	free(ptr);
}

t_list		tokenize_line(char *buff)
{
	char	**commands;
	char	**ptr;
	t_list	*list;

	list = NULL;
	commands = ft_strsplit(buff, ';');
	ptr = commands;
	while (*commands != NULL)
	{
		check_pipe(*commands, &list);
		free(*commands);
		commands++;
	}
	free(ptr);
	return (list);
}
