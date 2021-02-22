/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfreitas <jfreitas@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 21:13:42 by jfreitas          #+#    #+#             */
/*   Updated: 2021/02/22 00:21:14 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	ope(t_list **env, t_command *cmd, void (func)(t_list *, t_command *))
//{
//	(func)(env, cmd);
//} ???? I DON'T KNOW HOW TO DO IT WITH POINTER TO FUNCITONS :/ HELLLLP
void	execute_command(t_list **env, t_command *cmd)
{
	if (ft_strcmp(cmd->command[0], "echo") == 0)
		echo_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "cd") == 0)
		cd_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "pwd") == 0)
		pwd_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "export") == 0)
		export_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "unset") == 0)
		unset_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "env") == 0)
		env_builtin(env, cmd);
	else if (ft_strcmp(cmd->command[0], "exit") == 0)
		exit_builtin(env, cmd);
	//else if cmd == $ ??
	else
		executable_builtin(env, cmd);
}

/*
 ** Printing the propt by searching for the SESSION_MANAGER environmetal
 ** variable that is inside of the char **ep argument of the main.
 ** Then spliting the line by / and : just to get the exact name to then print
 ** it on the stdout (fd1).
 */
int		prompt(t_list *env, t_command *cmd)
{
	char	*find_env;
	char	**line;
	char	**session_manager;

	if ((find_env = find_env_value(&env, "SESSION_MANAGER")) == NULL)
	{
		ft_putstr_fd("minishell$ ", cmd->fd[1]);
		return (0);
	}
	else
	{
		find_env = ft_strchr(find_env, '/');
		line = ft_split(find_env, '/');
		session_manager = ft_split(line[0], ':');
		ft_putstr_fd("\033[1;32m", cmd->fd[1]);
		ft_putstr_fd(session_manager[0], cmd->fd[1]);
		ft_putstr_fd("% ", cmd->fd[1]);
		ft_putstr_fd("\033[0m", cmd->fd[1]);
		free(line);
		free(session_manager);
		return (0);
	}
}

int		gnl_ctrld(int fd, char **line)
{
	static char		buf[MAX_FD][BUFFER_SIZE + 1];
	char			*adr;

	if (BUFFER_SIZE < 1 || fd < 0 || !line ||
			fd > MAX_FD || read(fd, buf[fd], 0) == -1)
		return (-1);
	if (!(*line = ft_strnew(0)))
		return (0);
	while (!(adr = ft_strchr(buf[fd], '\n')))
	{
		if (!(join_newstr(line, buf[fd])))
			return (-1);
		ft_memset(buf[fd], 0, BUFFER_SIZE);
		if (!(read(fd, buf[fd], BUFFER_SIZE)))
		{
			if (**line)
				buf[fd][0] = 0;
			else
				return (0);
		}
	}
	*adr = 0;
	if (!(join_newstr(line, buf[fd])))
		return (-1);
	ft_strncpy(buf[fd], adr + 1, sizeof(buf[fd]));
	return (1);
}

static int	*init_fd_dev(void)
{
	int		*fd;

	fd = malloc(sizeof(int) * 4);
	bzero(fd, sizeof(int) * 4);
	fd[0] = 0;
	fd[1] = 1;
	fd[2] = 2;
	return (fd);
}

/*
 * This loop should only exit on ctrlD, sigkill and "exit" //  WHY SIGKILL?? 
 * It should be able to refuse to exit on Ctrl-d if needed (it does not exit
 * only when something is typed but the ctrlD is typed before the return button
 * - otherwise it\ll exit, either the command or the shell)
 * gnl return:
 * 1 = line read,
 * 0 = EOF
 * -1= error
 */
int		main_loop(t_list *env)
{
	char	*line;
	int		ret_gnl;
	////////////////////////joy's tests - initialization of t_command
	t_command	*cmd;

	cmd = (t_command*)malloc(sizeof(t_command) + 1);
	cmd->fd = init_fd_dev();
//	cmd->fd = (int*)malloc(sizeof(int) * 4);
//	cmd->fd[0] = 0;
//	cmd->fd[1] = 1;
//	cmd->fd[2] = 2;
	///////////////////////////

	ret_gnl = 1;
	prompt(env, cmd);

	// As ctrlC will just pr:int ^C and go to the new line AT ANYTIME, we
	// can handle
	// it inside the loop for when:
	// 1. a blocking command is typed, for example: grep h
	// 2. if something is typed and ctrlD is typed before the return button,
	// it writes ^C and goes to new line/promt
	// 3. for when command line is empty and no return button pressed
	// (crtlC is pressed instead)
	signal(SIGINT, ctrl_c_handler);
	//If the disposition is set to SIG_DFL, then the default action 
	//associated with the signal (see signal(7)) occurs.
	signal(SIGQUIT, ctrl_back_slash_handler);
	// Before the user input something, if ctrl\ is typed, nothing happpens
	// Inside the loop, if ctrl\ is typed, it has a different comportament)
	// 1. if a blocking command is typed, ctrl\ kils the comman with a
	// (core dumped) message and goes to a new line - i.e.: shows the prompt)
	// 2. if something is typed and ctrl\ is typer before the return button, it
	// does nothing

	while ((ret_gnl = gnl_ctrld(0, &line)) == 1)
	{
		if (!line[0])// ret_gnl = 1 when \n
		{
			free(line);
			prompt(env, cmd);
			continue ;
		}

		//printf("------------------call tokenize_line funciton and so on------------------\n\n");
		//	tokenize_line(line);//
		// inside this tokenize_line function -> to do:
		// 1.split it by | or ; or > or < or >>  and save it to the
		// t_cmd cmd->command (multiples of the same redirection has to fail) -
		// try to mix pipes and redirections 
		// 2.0. handle single quotes - try single quotes at arguments of
		// functions (also quoted empty args '', or whitespace, or ';' or and
		// also environment variables)
		// 2.1. and double quotes (and weird use of \ for double quotes, and
		// enrironment variabes - NO need to try multilines)
		// 3. handle $ENV_VAR (also with double quotes ex: echo "$USER")
		// 4. handle $? and send it to a function

		////////////////////////joy's tests - initialization of t_command

		cmd->command = ft_split(&line[0], ' ');

		///////////////////////
		execute_command(&env, cmd); // has to be called to find if the
		//command is a builtin and execute it, otherwise, execute command in
		//execve (using absolute path ->ex: /bin/ls or without path ->ex: ls or
		//relative path)
		//    ->  Also deal with the errors for those functions

		prompt(env, cmd);

		// void free_command_list(t_list **command) and
		// ft_lstdel(&env, free_env); -> at the very end of everything???
		// inside the loop????
		free(line);//free t_cmd function here??
		//	free(cmd);
		line = NULL;
	}
	free(line);
	line = NULL;

	// Before the user input something, if ctrlD
	//is typed, the string "exit" is written after the prompt, then the
	//shell closes, and $SHLVL environment variable is decreased

	// after any input, if ctrlD is typed, it has a different comportament:
	// 1. if a blocking command is typed, ctrlD stops the command
	// and goes to a new line - i.e.: shows the prompt)
	// 2. if something is typed and ctrlD is typed before the return button,
	// it does nothing
	if (ret_gnl == 0)
		ctrl_d_handler(line);

	return (0);
}

