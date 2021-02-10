#include "minishell.h"

int main(int ac, char *av[], char **ep)
{
	if (ac > 1)
		printf("%s", *av);
    t_list    *list;
    list = create_env_list(ep);
	t_command *cmd;
	int			i = 0;

	cmd = malloc(sizeof(t_command));
	cmd->command = NULL;
	cmd->command = malloc(sizeof(char *) * 6);
	bzero(cmd->command, (sizeof(char *) * 6));
	while (i < 3)
	{
		cmd->fd[i] = i;
		i++;
	}

	cmd->command[0] = "tata";

	env_builtin(&list, cmd);
	cmd->command[1] = "TEST=TOTO";
	export_builtin(&list, cmd);
	cmd->command[1] = "TEST=RITOTO";
	export_builtin(&list, cmd);
	cmd->command[1] = "TATA=tutu";
	export_builtin(&list, cmd);
	cmd->command[1] = "TITI=tutu";
	export_builtin(&list, cmd);


	env_builtin(&list, cmd);

    ft_lstdel(&list, free_env);
	cmd->command[0] = "-nnnnnn";
	cmd->command[1] = "-n";
	cmd->command[2] = "nice";
	cmd->command[3] = "yop";
	cmd->command[4] = "lulz";
	echo_builtin(NULL, cmd);
	cmd->command -= 5;
	cmd->command[0] = "n";
	echo_builtin(NULL, cmd);
    return(0);
}
