#include "minishell.h"

#define LINE(NAME) "\n======================-"#NAME"-========================\n"
#define LINE2 "-----------------\n"

/*
 * tokenize_line (func at the bottom) starts the tokenize process.
 * Priority spliter order : 1 - ;
 * 							2 - |
 * 							3 - >, >>, <<
 * Then $ expansion comes (if it's not inside simple quotes '')
 *
 * A -	Split the command line depending on ; char, if it's not inside quotes.
 * B -	Process each lines to find pipes, then redirection, which will result to
 * 		give them pipe() created fd, or open() created fd.
 * C -	Called by B(), expand $
 *
 *
 * -----------------------------------------------------------------------------
 *
 *
 * Returns an adress to the head of a t_list{} linked list, with following
 * content inside:
 *
 *
 * 		- t_command structure which contains itself 1-> an array of strings
 * 													2-> an array of int
 *
 *
 * 1- char	**command	-	command[0] must contains the name of executable.
 * 							This name could include the path, which should
 * 							be parsed by the executor function.
 * 						-	command[n] must contains args passed to the exe.
 * 					 	-	command[last] = NULL;
 * 2- int	*fd			-	will be malloc() as an array of 3 int.
 * 						-	fd[0] - fd from which exe reads.
 * 						-	fd[1] - fd to which exe write.
 * 						-	fd[2] - fd to which exe write errors.
 *
 *
 * -----------------------------------------------------------------------------
 *
 *
 * To redirect fd to which called exe will read or write (to/from a file fd or
 * to/from a pipe fd (=stream)) we will use the following way :
 * - dup2(open_fd, standard_fd)
 * It permits to make the system fd reference to our non-standard fd, which
 * had been opened to reference a file, or a pipe(),  instead of screen or
 * keyboard.
 *
 * An other way to see this :
 * an executable is coded to write to an object (screen) which is always pointed
 * by fd=1. We can't change the number of the fd which is used by the
 * executable, but we can change the object the fd=1 points to.
 * 
 * So instead of pointing to screen object, we can make fd=1 points to a file
 * object, or a pipe object.
 *
 * -----------------------------------------------------------------------------
 *
 * int	execve(char *path/to/exe, char **args, char **ev);
 *
 * Then, execution process will use the content of each link as this :
 * 
 * 1 - Parse head->content->command[0] to extract the path & name of the exe
 * 2 - If head->content->fd[0], [1], [2]  != 0, 1, 2 -> corresponding
 *     standard fd must be redirected with the technique described above
 * 3 - Fork a child which will execute the exec with execve();
 */

/*
 * parse_token_error handle some error cases for pipe like thoses lines :
 * 	1-	|ls|rev			>| >ls| >rev
 * 	2-	ls|rev| |wc		>ls| >rev| > | >wc
 * 	3-	ls|||rev		>ls| >| >| >rev
 * 	4-	ls||			>ls| >|					-> multiline not handled.
 * 	5-	ls|rev| 		>ls| >rev|				-> multiline not handled.
 * 	These error cases give "bash: syntax error near unexpected token `|'" 
 *
 * 	6-	ls|rev||wc		>ls| >rev| >| >wc		-> 'or' not handled.
 * 	In the case above, we have a logical 'or' which we don't handle, but we
 * 	still must make the commands before 'or' works. In this case the function
 * 	replaces the first character of all following splited lines by '\0' to make
 * 	them invisible & by this way not usable.
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
 * Process the given line to split it, if a '|' is found.
 * Behavior : if it receives this >ls | rev | wc -c
 * Make this :	->ls |'\0'
 * 				-> rev |'\0'
 * 				-> wc -c'\0'
 * 				->NULL
 *
 * If it receives this >ls | rev | wc -c|
 * Make this :	->ls |'\0'
 * 				-> rev |'\0'
 * 				-> wc -c|'\0'
 * 				->'\0'
 * 				->NULL
 *
 * It ignores '|' which are between two same quotes.
 * It handle distribution of piped file descriptors to commands & call
 * the find_redirections function which handles distribution of opened-file
 * file descriptor and $ expand.
*/

void		find_pipe_n_redirections(t_listjb **cmd, t_list **env,
		char *execution_line)
{
	char	**piped_exec_line;
	int		piped_fd[2];
	int		*fd_command;
	int		fd_tmp;
	int		i;

	delete_remaining_char(execution_line, ';');
	if (!(piped_exec_line = split_with_exception(execution_line, '|', "\'\"")))
		return ;
	i = 0;
	printf(LINE(FIND_PIPE_N_REDIRECTIONS));
	printf("execution_line RX: |%s|\n", execution_line);
	while (piped_exec_line[i])
	{
		if (!parse_token_error(piped_exec_line, i))
			return ;//token error
		if (!(fd_command = init_fd()))
			return ;//malloc error
		if (piped_exec_line[i + 1])//if there is a cmd which follows, open a pipe
		{// & write into
			pipe(piped_fd);//create pipe between current cmd and next cmd
			//printf("At %d time, thoses fd had been opened by pipe ->READ=%d\tWRITE=%d\n", i, piped_fd[0],piped_fd[1]);
			fd_command[1] = piped_fd[1];//current command write into pipe
		}
		if (i > 0)//if a string exists before, read from previous saved pipe
			fd_command[0] = fd_tmp;//read from the pipe opened by previous command
		printf(LINE2);
		printf("piped_exec_line[%d]: >%s<\n", i, piped_exec_line[i]);
		printf("command of string %d\tfd READ= %d\n", i, fd_command[0]);
		printf("command of string %d\tfd WRITE= %d\n\n", i, fd_command[1]);
		find_redirections(cmd, env, piped_exec_line[i], fd_command);
		fd_tmp = piped_fd[0];//save the read end pipe of current command
		i++;
	}
}

t_listjb		*tokenize_line_jb(char *line, t_list **env)
{
	int		i;
	t_listjb	*cmd;
	char	**execution_lines;
	char	*skiped;

	cmd = NULL;
	printf(LINE(TOKENIZE_LINE_JB));
	printf("line RX: |%s|\n", line);
	if (!(execution_lines = split_with_exception(line, ';', "\'\"")))
		return (NULL);
	i = -1;
	while (execution_lines[++i])
	{
		//printf("execution_line : |%s|\n", execution_lines[i]);
		//if one *line == ';' after only spaces or nothing
		//-> unexpected token ';'
		skiped = skip_char(execution_lines[i], ' ');
		if (*skiped == ';')
			printf("unexpected token ';'\n");
		printf(LINE2);
		printf("execution_line TX: |%s|\n", execution_lines[i]);
		find_pipe_n_redirections(&cmd, env, execution_lines[i]);
	}
	return (cmd);
}
