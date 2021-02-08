#include "minishell.h"

int main(int ac, char *av[], char **ep)
{
	if (ac > 1)
		printf("%s", *av);
    t_list    *list;
    list = create_env_list(ep);
	t_command *cmd;

	cmd = malloc(sizeof(t_command));
	cmd->command = NULL;
	cmd->args = malloc(sizeof(char *) * 6);
	bzero(cmd->args, (sizeof(char *) * 6));
	cmd->fd[1] = 1;

	export_builtin(&list, cmd);
    cmd->args[0] = "TEST=TOTO";
	export_builtin(&list, cmd);
    cmd->args[0] = "TRUC=";
	export_builtin(&list, cmd);
    cmd->args[0] = "MACHIN";
	export_builtin(&list, cmd);
    cmd->args[0] = "TEST=RITOTO";
	export_builtin(&list, cmd);
    cmd->args[0] = "TITI=TOTO";
	export_builtin(&list, cmd);
    printf("this is the Value for TITI: %s\n", find_env_value(&list, "TITI"));

    cmd->args[0] = "TITI";
	unset_builtin(&list, cmd);
    cmd->args[0] = "NEW";
	unset_builtin(&list, cmd);
	env_builtin(&list, cmd);

    ft_lstdel(&list, free_env);

	cmd->args[0] = "-nnnnnn";
	cmd->args[1] = "-n";
	cmd->args[2] = "nice";
	cmd->args[3] = "yop";
	cmd->args[4] = "lulz";
	echo_builtin(NULL, cmd);
	cmd->args -= 5;
	cmd->args[0] = "n";
	echo_builtin(NULL, cmd);
    return(0);
}
