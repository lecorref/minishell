#include "minishell.h"

void			print_list(void *content)
{
	printf("###print_list###\n");
	printf("|%s|\n", (char*)content);
	fflush(stdout);
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

int				init_fr(char **line_ptr, t_command *cmd, t_list **arg)
{
	*line_ptr = cmd->unexpanded;
	*arg = NULL;
	delete_remaining_char(*line_ptr, '|');
	return (1);
}

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

void			add_arg_to_list(t_list **arg, char *wobj)
{
	t_list		*new;

	if (!*wobj)
	{
		free(wobj);
		return ;
	}
	new = ft_lstnew(wobj);
	ft_lstadd_front(arg, new);
}

char			**convert_str_linkedlist_to_str_array(t_list *arg)
{
	int			size_list;
	char		**str_arr;

	size_list = ft_lstsize(arg);
	str_arr = (char **)malloc(sizeof(char*) * (size_list + 1));
	str_arr[size_list] = NULL;
	while (arg)
	{
		str_arr[--size_list] = arg->content;
		arg = arg->next;
	}
	return (str_arr);
}

//convertir list chainée de chaine de caractere en tableau de string
//delete la LL de str
//Rajouter arr_str dans le contenu i_command
//créer un nouveau maillon avec i_command en contenu
int				add_arglist_to_cmd(t_command *i_command, t_list **arg)
{
	char		**str_arr;

	str_arr = convert_str_linkedlist_to_str_array(*arg);
	ft_lstclear(arg, &clear_arglist);
	i_command->command = str_arr;
	return (1);
}
	//print_array(str_arr);

int				expander(t_list **env, t_command *i_command)
{
	char		*line_ptr;
	char		*word_object;
	t_list		*arg;

	if (!init_fr(&line_ptr, i_command, &arg))
		return (0);
	while (*line_ptr)
	{
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr && (*line_ptr == '>' || *line_ptr == '<'))
			if (!(redirections(env, &line_ptr, i_command)))
				return (0);
		line_ptr = skip_char(line_ptr, ' ');
		if (*line_ptr && (*line_ptr == '>' || *line_ptr == '<'))
			continue;
		if (!(word_object = expand_filename(env, &line_ptr)))
			return (0);
		add_arg_to_list(&arg, word_object);
	}
	add_arglist_to_cmd(i_command, &arg);
	return (1);
}

	//ft_lstiter(arg, &print_list);
/*
	printf(LINE(FIND_REDIRECTIONS));
	printf("command_line RX : |%s|\n", command_line);
**
	printf(LINE(FIND_REDIRECTIONS));
	printf("command_string TX to split: |%s|\n", command_string);
	print_array(command_array);
*/
