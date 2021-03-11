#include "minishell.h"

/*
** Because when ctrlD or exit builtinn is typed, they close the shell and
** decrease this $SHLVL environmental variable. Even though this $SHLVL env
** does not close the shell/window when its exported to any number, I think
** that it exists just for the record to know how many levels of shell there
** are currenclty in the current tab/window.
*/
int		increase_shlvl(t_list **env)
{
	char	*keyvalue;;
	char	*shlvl_str;
	int		shlvl_nb;

	if (!(shlvl_str = find_env_value(env, "SHLVL")))
	{
		add_env_variable(env, "SHLVL=1");
		return (0);
	}
	shlvl_nb = ft_atoi(shlvl_str);
	shlvl_nb++;
	if (!(shlvl_str = ft_itoa(shlvl_nb)))
		return (-1);
	if (!(keyvalue = ft_strjoin("SHLVL=", shlvl_str)))
	{
		ft_strdel(&shlvl_str);
		return (-1);
	}
	ft_strdel(&shlvl_str);
	add_env_variable(env, keyvalue);
	ft_strdel(&keyvalue);
	return (0);
}

char	**export_env(char **ep)
{
	char	**export_env_tab;
	int		i;
	int		j;

	i = 0;
	j = 0;
	export_env_tab = (char**)malloc(sizeof(char*) * ft_count_tab(ep) + 1);
	while (ep[i])
	{
		if (ep[i][0] != '_')
			export_env_tab[j++] = ft_strdup(ep[i]);
		i++;
	}
	export_env_tab[j] = NULL;
	return (export_env_tab);
}

int		main(int ac, char **av, char **ep)
{
	t_list	*env;
	t_list	*export;
	char	**export_env_tab_alpha_order;

	(void)av;
	if (ac != 1)
		return (-1);
	env = create_env_list(ep);
	export_env_tab_alpha_order = export_env(ep);
	export = create_env_list(export_env_tab_alpha_order);
	increase_shlvl(&env);
	g_exit_status = 0;
	g_line_eraser = 0;
	main_loop(&env, &export);
	ft_freetab(export_env_tab_alpha_order);
	ft_lstclear(&export, &clear_envlist);
	system("leaks minishell");
	return (g_exit_status);
}
