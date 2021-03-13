#include "minishell.h"

int				remove_quotes(char **str)
{
	char		*sub;
	int			len;
	
	if (**str != '\'' && **str != '\"')
		return (1);
	len = ft_strlen(*str);
	if (!(sub = ft_substr(*str, 1, len - 2)))
		return (0);
	free(*str);
	*str = sub;
	return (1);
}

/*
** expand_filename() is here to deal with tricky tricky filename like :
** > "HELLO$USER"THIS'is'still"a file name"'which is'solongand$USELESS
**
** yep, the string above could be only one filename.
*/
int		join_newstr_v2(char **str, const char *src)
{
	char	*tmp;
	int		len;

	if (!*str)
		if (!(*str = ft_strnew(0)))
			return (0);
	len = ft_strlen(*str) + ft_strlen(src);
	if (!(tmp = ft_strnew(len)))
		return (0);
	len = -1;
	while ((*str)[++len])
		tmp[len] = (*str)[len];
	while (*src)
		tmp[len++] = *src++;
	free(*str);
	*str = tmp;
	return (1);
}

char			*expand_filename(t_list **env, char **line_ptr)
{
	char		*word_object;
	char		*filename;

	filename = NULL;
	while (**line_ptr && !is_symbol_v2(**line_ptr))
	{
		if (**line_ptr && (**line_ptr == '\'' || **line_ptr == '\"'))
		{
			if (!(word_object = quotes(env, line_ptr)))
				return (NULL);
			if (!remove_quotes(&word_object))
				return (NULL);
		}
		else
			word_object = no_quotes(env, line_ptr);
		if (word_object)
		{
			if (!(join_newstr_v2(&filename, word_object)))
				return (NULL);
			free(word_object);
		}
	}
	return (filename);
}

	//printf("filename : |%s|\n", filename);
/*
** open_file() handle file opening thanks to open_code which says which flag
** to give to open & file name 'file', processed just before.
**
** If an error occurs while opening, errno is recorded in fd_command[3], to be
** used at execution time to set the error message. So, even if open() fails,
** processing continues, error just will be displayed after.
*/

static int		open_file(int open_code, int *fd_command, char *file)
{
	if (open_code == 3)
	{
		if (fd_command[0] != 0)
			close(fd_command[0]);
		if ((fd_command[0] = open(file, O_RDONLY)) == -1)
			fd_command[3] = errno;
		return (1);
	}
	if (fd_command[1] != 1)
		close(fd_command[1]);
	if (open_code == 1)
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd_command[1] == -1)
		fd_command[3] = errno;
	return (1);
}

/*
** redirections() translate the symbol of the first character from the given
** line ('<' , '>') to a code to how to use open().
** It then search for the first character of the string which must be the
** filename, this string will be highly processed by expand_filename() to
** give the expanded string (if necessary) as parameter to open().
** It must expand the dollar character '$', quotes, and also translate tilde.
** Once all data is fetched to use open accordingly, redirections() records the
** given file descriptor in the the appropriate int array which had been given
** from the t_command struct in parameter.
** It modifies the address where the given pointer line_ptr points to the end
** of the processed string.
*/

int				redirections(t_list **env, char **line_ptr, t_command *i_cmd)
{
	int			open_code;
	char		*file;

	if (**line_ptr == '>')
		open_code = 1;
	else
		open_code = 3;
	if (*(*line_ptr + 1) == '>' && ++open_code)
		*line_ptr += 1;
	*line_ptr = skip_char((*line_ptr + 1), ' ');
	if (!(file = expand_filename(env, line_ptr)))
		return (RT_FAIL);
	open_file(open_code, i_cmd->fd, file);
	if (i_cmd->file)
		free(i_cmd->file);
	i_cmd->file = file;
	if (i_cmd->fd[3] != 0)
		return (RT_BAD_FD);
	return (1);
}
