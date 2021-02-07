#include "minishell.h"

/*int main(int ac, char *av[], char **ep)
{
	if (ac > 1)
		printf("%s", *av);
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

    return(0);
}*/
