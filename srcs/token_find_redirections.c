#include "minishell.h"

void			print_array(char **array)
{
	int			i;

	i = -1;
	while (array[++i])
		printf("array[%d] :|%s|\n", i, array[i]);
	printf(LINE2);
}

char			*quotes(t_list **env, char **line_ptr)
{
	char		*word_object;

	if (**line_ptr == '\'')
		word_object = simple_quotes(line_ptr);
	else
		word_object = double_quotes(env, line_ptr);
	if (!word_object)
		return (NULL);
	return (word_object);
}

int				init_fr(char **line_ptr, char **command_line,
		char **command_string)
{
	*line_ptr = *command_line;
	if (!(*command_string = ft_strnew(0)))
		return (0);
	delete_remaining_char(*command_line, '|');
	return (1);
}

int				join_word_object(char **command_string, char **word_object)
{
	if (!*word_object)
		return (0);
	if (!(join_newstr(command_string, *word_object)))
		return (0);
	free(*word_object);
	*word_object = NULL;
	if (!(join_newstr(command_string, " ")))
		return (0);
	return (1);
}

int				create_array_n_link(t_list **cmd, int	*fd_command,
		char **command_string)
{
	char		**command_array;
	t_command	*command;	
	t_list		*new;	

	if (!(command_array = split_with_exception_v2(*command_string, ' ', "\'\"")))
		return (0);
	free(*command_string);
	if (!(command = (t_command*)malloc(sizeof(t_command) * 1)))
		return (0);
	command->command = command_array;
	command->fd = fd_command;
	if (!(new = ft_lstnew(command)))
		return (0);
	ft_lstadd_back(cmd, new);
	return (1);
}

//printf(LINE(ARRAY SPACES));
//print_array(command_array);
/*
** -----------------------------------------------------------------------------
**
** %    echo     BLA    >   FILE1   POP  >   FILE2      FUF    -n
** % cat FILE2
** --->BLA POP FUF -n
**
** %  echo   "L  BLA   L" >FILE1 POP>FILE2       "L    FUF    L"       -n
** % cat FILE2
** --->L  BLA   L POP L    FUF    L -n
**
** % cat    -b  < FILE   -n       <=>       % cat -b -n < FILE
**
** % >FILE
** ---> just open the FILE (create if it doesn't exists & erase content if it
** exists)
**
** -----------------------------------------------------------------------------
**
** find_redirections is the last fonction which is called to finish the tokenize
** process. In fact, this is where the content of links from linked list ends up
** being created.
** This function handles file descriptors from redirections, then shaping of
** the array of command + arguments.
**
** Behavior :
** func must skip spaces. The first string met should be the name of the exe, so
** command[0].
** However, the name of the exe can be placed after the redirection, so that we
** have to have the following logic :
** - If a redirection symbol is met, the following string will be its filename.
**   Else, it will be joined to the command_string, which will then have to be
**   splited with spaces to make an array understandable to execution part.
**
** Now, after skiping spaces, we have to check the first character.
** - If this is double quotes, go into double_quote() ('$' expanded).
** - Or else if it is a simple quote, go into simple_quote().
** - Or into no_quotes() if no quotes ('\' ignored).
** A - Thoses functions treats the string differently,
** & returns a malloc() string.
**
** - Go into redirections() if it's a redirection symbol.
** B - This function opens file descriptor consequently & address it
** to fd_command
**
** A() & B() funcs receives the address of pointer to string so that they can
** make it points to the end of word-object from string after processing.
**
** By this way line_ptr keeps track of address after the end of a word object.
** It permits to continue the loop after a word object has been processed.
**
** After word_object-processing functions layer, we fetched in word_object a
** malloc() string which contains a word-object.
** We have to join this new word-object with the command_string.
**
** When the command_line has been fully processed through the line_ptr copy, we
** get outside of the loop and then can split the command_string with 'sp' to
** create an array which will be pointed by a new link of our linked list.
**
**
** t_list **cmd :		Pointer to t_list from the tokenize_line func which is
** 						initially NULL and which is here to point to new links.
** 						As we have the address of this pointer, we can
** 						make it point to a newly created link.
**
** t_list **env :		pointer to t_list, which contains environment. Used to
** 						expand environment variables symbolized with '$'
**
** char *command_line :	String already processed twice to creates semi-colon ';'
**						& pipes '|' object. At this stage, this string can
**						contains redirections, otherwise it's clean and only
**						contains commands (which can still have to be expanded).
**
** int *fd_command :	Array of int which has been malloc() & initialized just
**						before.
**						Either they have already been changed to piped fd,
**						either not, & they could be changed here by redirection.
**						The address pointed by this variable will be fetched
**						by the pointer to int created in the t_command struture
**						which will be malloc in the function which creates links
*/
int				find_redirections(t_list **cmd, t_list **env,
		char *command_line, int *fd_command)
{
	char		*line_ptr;
	char		*word_object;
	char		*command_string;

	if (!init_fr(&line_ptr, &command_line, &command_string))
		return (0);
	while (*line_ptr)
	{
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr && (*line_ptr == '>' || *line_ptr == '<'))
			if (!(redirections(env, &line_ptr, fd_command)))
				return (0);
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr && (*line_ptr == '\'' || *line_ptr == '\"'))
			word_object = quotes(env, &line_ptr);
		else if (*line_ptr && (*line_ptr != '<' && *line_ptr != '>'))
			word_object = no_quotes(env, &line_ptr);
		else
			continue;
		if (!(join_word_object(&command_string, &word_object)))
			return (0);
	}
	if (!(create_array_n_link(cmd, fd_command, &command_string)))
		return (0);
	return (1);
}

/*
	printf(LINE(FIND_REDIRECTIONS));
	printf("command_line RX : |%s|\n", command_line);
**
	printf(LINE(FIND_REDIRECTIONS));
	printf("command_string TX to split: |%s|\n", command_string);
	print_array(command_array);
*/
