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

void	print_cmd(t_command *cmd)/////////delete
{
	int	i;

	printf("\n----------TESTING PURPOSES----------\n");
	printf("\t\t\t\tfd[0] : %d\tfd[1] : %d\n", cmd->fd[0], cmd->fd[1]);
	printf("\t\t\t\tfile : %s\n", cmd->file);
	i = -1;
	while (cmd->command[++i])
		printf("cmd->command[%d] = %s\n", i, cmd->command[i]);
	printf("----------TESTING PURPOSES----------\n\n");
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
