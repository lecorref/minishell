#include "minishell.h"

/*
** parse_token_error() handle some error cases for pipe like thoses lines :
** 	1-	|ls|rev			>| >ls| >rev
** 	2-	ls|rev| |wc		>ls| >rev| > | >wc
** 	3-	ls|||rev		>ls| >| >| >rev
** 	4-	ls||			>ls| >|					-> multiline not handled.
** 	5-	ls|rev| 		>ls| >rev|				-> multiline not handled.
** 	These error cases give "bash: syntax error near unexpected token `|'" 
**
** 	6-	ls|rev||wc		>ls| >rev| >| >wc		-> 'or' not handled.
** 	In the case above, we have a logical 'or' which we don't handle, but we
** 	still must make the commands before 'or' work. In this case the function
** 	replaces the first character of all following splited lines by '\0' to make
** 	them invisible & by this way not usable.
*/
int			parse_token_error(char **str, int i)
{
	char	*skiped;

	skiped = skip_char(str[i], ' ');
	if ((*skiped == '|' && i == 0) ||
			(*skiped == '|' && skiped != str[i] && i > 0))//1 & 2
		printf("1&2 unexpected token '|'\n");
	else if (i > 0 && *str[i] == '|')
	{
		skiped = skip_char(str[i + 1], ' ');
		if (*skiped == '|' || *skiped == 0)//3 & 4
			printf("3&4 unexpected token '|'\n");
		else//6 (logical 'or' -> 0th the 1st char of this string & folowings)
		{
			while (str[i])
				*str[i++] = 0;
			printf("6 processed\n");
		}
	}
	else if (!(*str[i]) && !str[i + 1] && i > 0)//5
		printf("5 unexpected token '|'\n");
	return (1);
}

/*
** pipe_it()
** The array received as first parameter is the array of splited string with
** pipe symbol '|'
** It means that each string represents a set of commmand which can have their
** reading and/or writing file descriptor changed to a piped file descriptor.
**
** If there is string after the current one, it means the current one must
** write into a pipe where the next one will read.
** Else, the current one can write into the standard fd.
**
** If i > 0, in other words, if their was a command before the current one, it
** means a pipe had been opened before to write into, so the current one must
** read into the read end pipe created before, which had been saved in fd_tmp.
*/
int				pipe_it(char **piped_exec_line, int i,
		int *fd_command, int *fd_tmp)
{
	int			piped_fd[2];

	if (piped_exec_line[i + 1])
	{
		if (pipe(piped_fd) == -1)
			return (0);
		fd_command[1] = piped_fd[1];
	}
	if (i > 0)
		fd_command[0] = *fd_tmp;
	*fd_tmp = piped_fd[0];
	return (1);
}

/*
** find_pipe_n_redirections()
** Process the given line to split it, if a '|' is found.
** Behavior : if it receives this >ls | rev | wc -c
** Make this :	->ls |'\0'
** 				-> rev |'\0'
** 				-> wc -c'\0'
** 				->NULL
**
** If it receives this >ls | rev | wc -c|
** Make this :	->ls |'\0'
** 				-> rev |'\0'
** 				-> wc -c|'\0'
** 				->'\0'
** 				->NULL
**
** It ignores '|' which are between two same quotes.
** It handle distribution of piped file descriptors to commands & call
** the find_redirections function which handles distribution of opened-file
** file descriptor and $ expand.
*/
void		print_array3(char **array)
{
        int                     i;

        i = -1;
        while (array[++i])
                printf("array[%d] :|%s|\n", i, array[i]);
        printf(LINE2);
}

int				find_pipe_n_redirections(t_list **cmd, t_list **env,
		char *execution_line)
{
	char		**piped_exec_line;
	int			*fd_command;
	int			fd_tmp;
	int			i;

	delete_remaining_char(execution_line, ';');
	if (!(piped_exec_line = split_with_exception(execution_line, '|', "\'\"")))
		return (0);
	i = -1;
	while (piped_exec_line[++i])
	{
		if (!parse_token_error(piped_exec_line, i))
			return (0);//token error
		if (!(fd_command = init_fd()))
			return (0);//malloc error
		if (!pipe_it(piped_exec_line, i, fd_command, &fd_tmp))
			return (0);
		if (!(find_redirections(cmd, env, piped_exec_line[i], fd_command)))
			return (0);
		free(piped_exec_line[i]);
	}
	free(piped_exec_line);
	return (1);
}

/*
	printf(LINE(FIND_PIPE_N_REDIRECTIONS));
	printf("execution_line RX: |%s|\n", execution_line);
	printf(LINE(ARRAY PIPE));
	print_array3(piped_exec_line);
		printf(LINE2);
		printf("command of string %d\tfd READ= %d\n", i, fd_command[0]);
		printf("command of string %d\tfd WRITE= %d\n\n", i, fd_command[1]);
*/

/*
** tokenize_line() starts the tokenize process.
** Priority spliter order : 1 - ;
** 							2 - |
** 							3 - >, >>, <
** Then $ expansion comes (if it's not inside simple quotes '')
**
** A -	Split the command line depending on ; char, if it's not inside quotes.
** B -	Process each lines to find pipes, then redirection, which will result to
** 		give them pipe() created fd, or open() created fd.
** C -	Called by B(), expand $
**
**
** -----------------------------------------------------------------------------
**
**
** Returns an adress to the head of a t_list{} linked list, with following
** content inside:
**
**
** 		- t_command structure which contains itself 1-> an array of strings
** 													2-> an array of int
**
**
** 1- char	**command	-	command[0] must contains the name of executable.
** 							This name could include the path, which should
** 							be parsed by the executor function.
** 						-	command[n] must contains args passed to the exe.
** 					 	-	command[last] = NULL;
** 2- int	*fd			-	will be malloc() as an array of 3 int.
** 						-	fd[0] - fd from which exe reads.
** 						-	fd[1] - fd to which exe write.
** 						-	fd[2] - fd to which exe write errors.
**
** -----------------------------------------------------------------------------
**
** To redirect fd to which called exe will read or write (to/from a file fd or
** to/from a pipe fd (=stream)) we will use the following way :
** - dup2(open_fd, standard_fd)
** It permits to make the system fd reference to our non-standard fd, which
** had been opened to reference a file, or a pipe(),  instead of screen or
** keyboard.
**
** An other way to see this :
** an executable is coded to write to an object (screen) which is always pointed
** by fd=1. We can't change the number of the fd which is used by the
** executable, but we can change the object the fd=1 points to.
**
** So instead of pointing to screen object, we can make fd=1 points to a file
** object, or a pipe object.
**
** -----------------------------------------------------------------------------
**
** int	execve(char *path/to/exe, char **args, char **ev);
**
** Then, execution process will use the content of each link as this :
**
** 1 - Parse head->content->command[0] to extract the path & name of the exe
** 2 - If head->content->fd[0], [1], [2]  != 0, 1, 2 -> corresponding
**     standard fd must be redirected with the technique described above
** 3 - Fork a child which will execute the exec with execve();
*/
t_list			*tokenize_line_jb(char *line, t_list **env)
{
	int			i;
	t_list		*cmd;
	char		**execution_lines;
	char		*skiped;

	cmd = NULL;
	if (!(execution_lines = split_with_exception(line, ';', "\'\"")))
		return (NULL);
	i = -1;
	while (execution_lines[++i])
	{
		skiped = skip_char(execution_lines[i], ' ');
		if (*skiped == ';')
			printf("unexpected token ';'\n");
		if (!(find_pipe_n_redirections(&cmd, env, execution_lines[i])))
			return (NULL);
		free(execution_lines[i]);
	}
	free(execution_lines);
	return (cmd);
}

/*
	printf(LINE(TOKENIZE_LINE_JB));
    printf(LINE(ARRAY SEMICOLON));
	print_array3(execution_lines);
	printf("execution_line TX: |%s|\n", execution_lines[i]);
	printf("line RX: |%s|\n", line);
		printf(LINE2);
*/
