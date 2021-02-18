#include "minishell.h"

/*
 * -----------------------------------------------------------------------------
 *
 * %    echo     BLA    >   FILE1   POP  >   FILE2      FUF    -n
 * % cat FILE2
 * --->BLA POP FUF -n
 *
 * %  echo   "L  BLA   L" >FILE1 POP>FILE2       "L    FUF    L"       -n
 * % cat FILE2
 * --->L  BLA   L POP L    FUF    L -n
 *
 * % cat    -b  < FILE   -n       <=>       % cat -b -n < FILE
 *
 * % >FILE
 * ---> just open the FILE (create if it doesn't exists & erase content if it
 * exists)
 *
 * -----------------------------------------------------------------------------
 *
 * find_redirections is the last fonction which is called to finish the tokenize
 * process. In fact, this is where the content of links from linked list ends up
 * being created.
 * This function handle file descriptors from redirections, then shaping of
 * the array of command + arguments.
 *
 * Behavior :
 * func must skip spaces. The first string met should be the name of the exe, so
 * command[0].
 * However, the name of the exe can be placed after the redirection, so that we
 * have to have the following logic :
 * - If a redirection symbol is met, the following string will be its filename.
 *   Else, it will be joined to the command_string, which will then have to be
 *   splited with spaces to make an array understandable to execution part.
 *
 * Now, after skiping spaces, we have to check the first character.
 * - If this is double quotes, go into double_quote() ('$' expanded).
 * - Or else if it is a simple quote, go into simple_quote().
 * - Or into no_quotes() if no quotes ('\' ignored).
 * A - Thoses functions treats the string differently,
 * & returns a malloc() string.
 *
 * - Go into symbolic() if it's a redirection symbol.
 * B - This function opens file descriptor consequently & adress it to fd_command
 *
 * A() & B() funcs receives the adress of pointer to string so that they can
 * make it points to the end of word-object from string after processing.
 *
 * By this way line_ptr keeps track of adress after the end of a word object.
 * It permits to continue the loop after a word object has been processed.
 *
 * After word_object-processing functions layer, we fetched in word_object a
 * malloc() string which contains a word-object.
 * We have to join this new word-object with the command_string.
 *
 * When the command_line has been fully processed through the line_ptr copy, we
 * get outside of the loop and then can split the command_string with 'sp' to
 * create an array which will be pointed by a new link of our linked list.
*/

char			*end_of_object(char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '>' || *str == '<')
			return (str);
		str++;
	}
	return (str);
}

void			symbolic(char **line_ptr, int *fd_command)
{
	int			open_code;
	char		*file;

	printf("into symbolic(). line received : |%s|\n", *line_ptr);
	if (**line_ptr == '>')
		open_code = 1;
	else
		open_code = 3;
	if (*(*line_ptr + 1) == '>' && ++open_code)
		*line_ptr += 1;
	file = skip_char((*line_ptr + 1), ' ');
	if (!*(*line_ptr = end_of_object(file)))//means filename at the end
		file = ft_substr(*line_ptr, 0, ft_strlen(file));
	else
		file = ft_substr(*line_ptr, 0, (*line_ptr - file));
	if (open_code == 3)
		fd_command[0] = open(file, O_RDONLY);
	else if (open_code == 1)
		fd_command[1] = open(file, O_WRONLY, O_CREAT);
	else
		fd_command[1] = open(file, O_WRONLY, O_CREAT | O_APPEND);
	free(file);
}

int				is_symbol(int c)
{
	return (c == '>' || c == '<' || c == ' ' || c == '\'' || c == '\"');
}

char			*doll_expand(t_list **env, char **line_ptr)
{
	char		*doll;
	char		*value;

	*line_ptr += 1;
	doll = *line_ptr;
	while (**line_ptr && !is_symbol(**line_ptr))
	{
		if (**line_ptr == '$')
		{
			*line_ptr += 1;
			break;
		}
		*line_ptr += 1;
	}
	if (!(doll = ft_substr(doll, 0, (*line_ptr - doll))))
		return (NULL);
	value = find_env_value(env, doll);
	free(doll);
	return (value);
}

char			*double_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*tmp;
	char		*final_str;
	char		*expanded;

	printf("into double_quotes(). line received : |%s|\n", *line_ptr);
	*line_ptr += 1;
	str = *line_ptr;
	final_str = ft_strnew(0);
	while (*str && *str !=  '\"')
	{
		if (*str == '$')
		{
			tmp = ft_substr(*line_ptr, 0, (str - *line_ptr));
			join_newstr(&final_str, tmp);
			free(tmp);
			expanded = doll_expand(env, &str);
			join_newstr(&final_str, expanded);
			*line_ptr = str;
			continue;
		}
		str++;
	}
	tmp = ft_substr(*line_ptr, 0, (str - *line_ptr));
	join_newstr(&final_str, tmp);
	free(tmp);
	*line_ptr = str + 1;
	printf("into double_quotes(). line returned : |%s|\n", final_str);
	return (final_str);
}

char			*simple_quotes(char **line_ptr)
{
	char		*word_object;
	char		*tmp;

	printf("into simple_quotes(). line received : |%s|\n", *line_ptr);
	*line_ptr += 1;
	tmp = *line_ptr;
	while (**line_ptr != '\'')
		*line_ptr += 1;
	if (!(word_object = ft_substr(tmp, 0, (*line_ptr - tmp))))
		return (NULL);
	*line_ptr += 1;
	printf("into simple_quotes(). line returned : |%s|\n", word_object);
	return (word_object);
}

char			*quotes(t_list **env, char **line_ptr)
{
	char		*word_object;

	if (**line_ptr == '\'')
		word_object = simple_quotes(line_ptr);
	else
		word_object = double_quotes(env, line_ptr);
	return (word_object);
}

char			*no_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*tmp;
	char		*final_str;
	char		*expanded;

	printf("into no_quotes(). line received : |%s|\n", *line_ptr);
	str = *line_ptr;
	final_str = ft_strnew(0);
	while (*str && !is_symbol(*str))
	{
		if (*str == '$')
		{
			tmp = ft_substr(*line_ptr, 0, (str - *line_ptr));
			join_newstr(&final_str, tmp);
			free(tmp);
			expanded = doll_expand(env, &str);
			join_newstr(&final_str, expanded);
			*line_ptr = str;
			continue;
		}
		str++;
	}
	tmp = ft_substr(*line_ptr, 0, (str - *line_ptr));
	join_newstr(&final_str, tmp);
	free(tmp);
	*line_ptr = str;
	printf("into no_quotes(). line returned : |%s|\n", final_str);
	return (final_str);
}

void			create_link(t_listjb **cmd, int *fd_command, char **command_array)
{
	t_command	*command;	

	command = (t_command*)malloc(sizeof(t_command) * 1);
	command->command = command_array;
	command->fd = fd_command;
	ft_lstadd_back_jb(cmd, ft_lstnew_jb(command));
}

void			find_redirections(t_listjb **cmd, t_list **env,
		char *command_line, int *fd_command)
{
	char		*line_ptr;
	char		*word_object;
	char		*command_string;
	char		**command_array;

	line_ptr = command_line;
	command_string = ft_strnew(0);
	printf("into find_redirections(). line received : |%s|\n", command_line);
	while (*line_ptr)
	{
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr == '>' || *line_ptr == '<')
			symbolic(&line_ptr, fd_command);//pcss until space/EOL
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr == '\'' || *line_ptr == '\"')
			word_object = quotes(env, &line_ptr);//process until corresponding quote
		else if (*line_ptr != '<' || *line_ptr != '>')
			word_object = no_quotes(env, &line_ptr);//process until space/EOL
		else
			continue;
		join_newstr(&command_string, word_object);
		free(word_object);
	}
	command_array = ft_strsplit(command_string, ' ');
	create_link(cmd, fd_command, command_array);
}
