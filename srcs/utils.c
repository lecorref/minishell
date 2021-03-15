#include "minishell.h"

char	*last_arg(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd->command[i])
		i++;
	if (i > 0)
		i -= 1;
	return (cmd->command[i]);
}

int		update_underscore(t_list **env, char *path_cmd)
{
	char	*keyvalue;

	if (!(keyvalue = ft_strjoin("_=", path_cmd)))
		return (1);
	add_env_variable(env, keyvalue);
	free(keyvalue);
	return (1);
}

int		ft_count_tab(char **tab)
{
	int i;

	i = 0;
	if (tab == NULL)
		return (0);
	while (tab[i] != NULL)
		i++;
	return (i);
}

void	print_cmd(t_command *cmd)/////////delete
{
	int	i;

	printf(LINE(COMMAND));
	printf("file : %s\t\t\tfd[0] : %d\tfd[1] : %d\n",
			cmd->file, cmd->fd[0], cmd->fd[1]);
	i = -1;
	while (cmd->command[++i])
		printf("cmd->command[%d] = |%s|\n", i, cmd->command[i]);
	printf("cmd->command[%d] = |%s|\n", i, cmd->command[i]);
	printf(LINE2);
	fflush(stdout);
}/////////delete

void	print_array(char **arr)
{
	int	i;

	printf("###printarray####\n");
	i = -1;
	while (arr[++i])
		printf("str[%d] : |%s|\n", i, arr[i]);
	printf("str[%d] : |%s|\n", i, arr[i]);
	fflush(stdout);
}

/*
** while the next index array of the env command exists, compare the current
** index with the next one (meaning envir[i] is a char bigger than envir[i + 1].
** ex B, A), then, swap the lines.
*/
char		**alpha_order_array(char **export_tab)
{
	char	*export_line;
	int		i;

	i = 0;
	export_line = NULL;
	while (export_tab[i + 1])
	{
		if (ft_strcmp(export_tab[i], export_tab[i + 1]) > 0)
		{
			export_line = export_tab[i];
			export_tab[i] = export_tab[i + 1];
			export_tab[i + 1] = export_line;
			i = 0;
		}
		else
			i++;
	}
	return (export_tab);
}
