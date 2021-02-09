/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 00:47:28 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/07 02:55:57 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		main(int ac, char **av, char **ep)
{
	t_list	*list;

	(void)av;
	if (ac != 1)
		return (-1);
	list = create_env_list(ep);
	//increase_shlvl(env);// CALL THIS FUNCTION TO INCRESE THE $SHLVL env??? Because when ctrlD or exit builtinn is typed, they close the shell and decrease this $SHLVL environmental variable. Even though this $SHLVL env does not close the shell/window when its exported to any number, I think that it exists just for the record to know how many levels of shell there are currenclty in the current tab/window.
	main_loop(list);
	//ft_lstdel(&list, free_env); ?????
	//free_command_list(t_list **command);??
	return (0);
}
