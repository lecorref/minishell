#include "minishell.h"

int main(int ac, char *av[], char **ep)
{
	if (ac > 1)
		printf("%s", *av);
	/* env function test
    char    **test;
    t_list    *list;
    list = create_env_list(ep);

    add_env_variable(&list, ft_strdup("TEST=TOTO"));
    add_env_variable(&list, ft_strdup("TEST=RITOTO"));
    add_env_variable(&list, ft_strdup("TATA=tutu"));
    add_env_variable(&list, ft_strdup("TITI=tutu"));
    printf("this is the Value for TITI: %s\n", find_env_value(&list, "TITI"));
    delete_env_variable(&list, "TITI");
    test = env_list_to_tab(list);

    ft_lstdel(&list, free_env);


    while (*test != NULL)
    {
        printf("%s\n", *test);
        test++;
    }
*/
	t_command *cmd;
	(void)ep;
	int			i = 0;

	cmd = malloc(sizeof(t_command));
	cmd->command = NULL;
	cmd->args = malloc(sizeof(char *) * 6);
	bzero(cmd->args, (sizeof(char *) * 6));
	while (i < 3)
	{
		cmd->fd[i] = i;
		i++;
	}
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
