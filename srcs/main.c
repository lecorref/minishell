/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 00:47:28 by jfreitas          #+#    #+#             */
/*   Updated: 2021/03/04 23:43:39 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int		main(int ac, char **av, char **ep)
{
	t_list	*env;
//	char	**env_tab;

	(void)av;
	if (ac != 1)
		return (-1);
	env = create_env_list(ep);
	//env_tab = env_list_to_tab(env);
	increase_shlvl(&env);// CALL THIS FUNCTION TO INCRESE THE $SHLVL env???
	//Because when ctrlD or exit builtinn is typed, they close the shell and
	//decrease this $SHLVL environmental variable. Even though this $SHLVL env
	//does not close the shell/window when its exported to any number, I think
	//that it exists just for the record to know how many levels of shell there
	//are currenclty in the current tab/window.
	g_exit_status = 0;
	g_line_eraser = 0;
	main_loop(&env);
	//ft_freetab(env_tab);
	//system("leaks minishell");
	return (errno);
}
