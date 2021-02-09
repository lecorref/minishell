/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 21:13:42 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/08 21:58:56 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	ope(t_list **head, t_command *cmd, void (func)(t_list *, t_command *))
//{
//	(func)(head, cmd);
//} ???? I DON'T KNOW HOW TO DO IT WITH POINTER TO FUNCITONS SO I DID AS DOWN BELOW LOL :/ HELLLLP
void	execute_command(t_list **head, t_command *cmd)
{
	while (cmd->commad)
	{
		if (ft_strcmp(cmd->command[0], "echo") == 0)
			echo_builtin(head, cmd);
		else if (ft_strcmp(cmd->command[0], "cd") == 0)
			cd_builtin(head, cmd);
		else if (ft_strcmp(cmd->command[0], "pwd") == 0)
			pwd_builtin(head, cmd);
		else if (ft_strcmp(cmd->command[0], "export") == 0)
			export_builtin(head, cmd);
		else if (ft_strcmp(cmd->command[0], "unset") == 0)
			unset_builtin(head, cmd);
		else if (ft_strcmp(cmd->comman[0]d, "env") == 0)
			env_builtin(head, cmd);
		else if (ft_strcmp(cmd->command[0], "exit") == 0)
			exit_builtin(head, cmd);
		else
			executables_builtin(head, cmd);
	}
}

/*
** Printing the propt by searching for the SESSION_MANAGER environmetal
** variable that is unside of the char **ep argument of the main.
** Then spliting the line by / and : just to get the exact name to then print
** it on the stdout (fd1).
*/
int		prompt(t_list *list)//or t_env *env?
{
	char	*find_env;
	char	**line;
	char	**session_manager;

	if ((find_env = find_env_value(&list, "SESSION_MANAGER")) == NULL)
		return (-1);
	else
	{
		find_env = ft_strchr(find_env, '/');
		line = ft_strsplit(find_env, '/');
		session_manager = ft_strsplit(line[0], ':');
		ft_putstr_fd(session_manager[0], 1);// change fd to cmd->fd[1]??
		ft_putstr_fd("% ", 1);
		free(line);
		free(session_manager);
		return (0);
	}
}

/*
 * This loop should only exit on ctrlD, sigkill and "exit" //  WHY SIGKILL?? 
 * It should be able to refuse to exit on Ctrl-d if needed (it does not exit only when something is typed but the ctrlD is typed before the return button - otherwise it\ll exit, either the command or the shell)
 * gnl return:
 * 1 = line read,
 * 0 = EOF
 * -1= error
*/
int		main_loop(t_list *list)// or t_env *env?
{
	char	*line;
	int		ret_gnl;

	prompt(list);
	signal(SIGQUIT, ctrl_back_slash_handler);// Before the user input something, if ctrl\ is typed, nothing happpens
// Inside the loop, if ctrl\ is typed, it has a different comportament)
// 1. if a blocking command is typed, ctrl\ kils the comman with a (core dumped) message and goes to a new line - i.e.: shows the prompt)
// 2. if something is typed and ctrl\ is typer before the return button, it does nothing

	while ((ret_gnl = get_next_line(0, &line)) != 1)
	{
		ctrl_d_handler();// Before the user input something, if ctrlD is typed, the string "exit" is written after the prompt, then the shell closes, and $SHLVL environment variable is decreased
// Inside the loop, if ctrlD is typed, it has a different comportament)
// 1. if a blocking command is typed, ctrlD stops the command and goes to a new line - i.e.: shows the prompt)
// 2. if something is typed and ctrlD is typed before the return button, it does nothing

// As ctrlC will just print ^C and go to the new line AT ANYTIME, we can handle it inside the loop for when:
// 1. a blocking command is typed, for example: grep h
// 2. if something is typed and ctrlD is typed before the return button, it writes ^C and goes to new line/promt
// 3. for when command line is empty and no return button pressed (crtlC is pressed instead)

// The exit builtin can be inside the get_next_line function since it will be called only after the user writes the word "exit". Then it'll do a new line, write the string "exit", then exit the shell and decrease the $SHLVL environment variable
	}

//	t_list  tokenize_line(char *line);
	//tokenize_line(line);// then use pointers to functions call the functions to built the 7 builtins, and the absolute path ones (the rest)
	printf("call tokenize_line funciton and so on\n");
// inside this tokenize_line function -> to do:
// 1.split it by | or ; or > or < or >>  and save it to the t_cmd??????????????
// 2. void execute_command(t_command *command, t_list **env); has to be called to find if the command is a builtin and execute it, otherwise, execute command in execve (using absolute path ->ex: /bin/ls or without path ->ex: ls)
//    2.1. Also deal with the errors for those functions
// 3. void free_command_list(t_list **command) and ft_lstdel(&list, free_env); -> at the very end of everything???
	
	execute_command(t_list **head, t_command *command);

	free(line);
//	free(line);??
//	free(session_manager);??
	return (0);
}
