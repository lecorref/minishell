/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 21:13:42 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/12 14:35:30 by jfreitas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	ope(t_list **head, t_command *cmd, void (func)(t_list *, t_command *))
//{
//	(func)(head, cmd);
//} ???? I DON'T KNOW HOW TO DO IT WITH POINTER TO FUNCITONS :/ HELLLLP
void	execute_command(t_list **head, t_command *cmd)
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
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		env_builtin(head, cmd);
	else if (ft_strcmp(cmd->command[0], "exit") == 0)
		exit_builtin(head, cmd);
	//else if cmd == $ ??
	else
		executable_builtin(head, cmd);
}

/*
** Printing the propt by searching for the SESSION_MANAGER environmetal
** variable that is inside of the char **ep argument of the main.
** Then spliting the line by / and : just to get the exact name to then print
** it on the stdout (fd1).
*/
int		prompt(t_list *head, t_command *cmd)
{
	char	*find_env;
	char	**line;
	char	**session_manager;

	if ((find_env = find_env_value(&head, "SESSION_MANAGER")) == NULL)
		return (-1);
	else
	{
		find_env = ft_strchr(find_env, '/');
		line = ft_strsplit(find_env, '/');
		session_manager = ft_strsplit(line[0], ':');
		ft_putstr_fd("\033[1;32m", cmd->fd[1]);
		ft_putstr_fd(session_manager[0], cmd->fd[1]);
		ft_putstr_fd("% ", cmd->fd[1]);
		ft_putstr_fd("\033[0m", cmd->fd[1]);
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
int		main_loop(t_list *list)
{
	char	*line;
	int		ret_gnl;
	////////////////////////joy's tests - initialization of t_command
	t_command	*cmd;

	cmd = (t_command*)malloc(sizeof(t_command));
	cmd->fd[0] = 0;
	cmd->fd[1] = 1;
	cmd->fd[2] = 2;
///////////////////////////

	ret_gnl = 1;
	prompt(list, cmd);
	//If the disposition is set to SIG_DFL, then the default action associated with the signal (see signal(7)) occurs.
	signal(SIGQUIT, ctrl_back_slash_handler);// Before the user input something, if ctrl\ is typed, nothing happpens
// Inside the loop, if ctrl\ is typed, it has a different comportament)
// 1. if a blocking command is typed, ctrl\ kils the comman with a (core dumped) message and goes to a new line - i.e.: shows the prompt)
// 2. if something is typed and ctrl\ is typer before the return button, it does nothing

	while (ret_gnl == 1)
	{
		ret_gnl = get_next_line_jb(0, &line);
		if (!line[0] && ret_gnl == 1)// ret_gnl = 1 when \n
		{
			free(line);
			prompt(list, cmd);
			continue ;
		}

//////////////////////////// ctrlD on a line that exists is NOT WORKING HERE

		if (ret_gnl == 0 && !line[0])// ctrlD handler if no line and gnl does not return 1
			ctrl_d_handler(line);// Before the user input something, if ctrlD is typed, the string "exit" is written after the prompt, then the shell closes, and $SHLVL environment variable is decreased
		if (ret_gnl == 0 && line)
		{
		//	here it should do nothing.. wait for return () to be pressed, so gnl would return 1 and the line could be then parsed and executed
			printf("\n->%d\n", ret_gnl);
		//	prompt(list, cmd);
		}

//////////////////////////// ctrlD on a line that exists is NOT WORKING HERE

// after any input, if ctrlD is typed, it has a different comportament:
// 1. if a blocking command is typed, ctrlD stops the command and goes to a new line - i.e.: shows the prompt)
// 2. if something is typed and ctrlD is typed before the return button, it does nothing
		//	continue ;

// As ctrlC will just pr:int ^C and go to the new line AT ANYTIME, we can handle it inside the loop for when:
// 1. a blocking command is typed, for example: grep h
// 2. if something is typed and ctrlD is typed before the return button, it writes ^C and goes to new line/promt
// 3. for when command line is empty and no return button pressed (crtlC is pressed instead)

// The exit builtin can be inside the get_next_line function since it will be called only after the user writes the word "exit". Then it'll do a new line, write the string "exit", then exit the shell and decrease the $SHLVL environment variable

		printf("------------------call tokenize_line funciton and so on------------------\n\n");
	//	tokenize_line(line);//
	// inside this tokenize_line function -> to do:
// 1.split it by | or ; or > or < or >>  and save it to the t_cmd cmd->command (multiples of the same redirection has to fail) - try to mix pipes and redirections 
// 2.0. handle single quotes - try single quotes at arguments of functions (also quoted empty args '', or whitespace, or ';' or and also environment variables)
// 2.1. and double quotes (and weird use of \ for double quotes, and enrironment variabes - NO need to try multilines)
// 3. handle $ENV_VAR (also with double quotes ex: echo "$USER")
// 4. handle $? and send it to a function

////////////////////////joy's tests - initialization of t_command

		cmd->command = ft_strsplit(&line[0], ' ');

///////////////////////
		if (ret_gnl == 1 && line)
			execute_command(&list, cmd); // has to be called to find if the command is a builtin and execute it, otherwise, execute command in execve (using absolute path ->ex: /bin/ls or without path ->ex: ls or relative path)
//    ->  Also deal with the errors for those functions

		prompt(list, cmd);

	// void free_command_list(t_list **command) and ft_lstdel(&list, free_env); -> at the very end of everything??? inside the loop????
		free(line);//free t_cmd function here??
	//	free(cmd);
		line = NULL;
	}
	free(line);
	return (0);
}

