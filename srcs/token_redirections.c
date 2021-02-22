#include "minishell.h"

/*
 * redirections() translate the symbol of the first character from the given
 * line to a code to how to use open().
 * It then search for the first character of the string which must be the
 * filename, and will be give as parameter to open().
 * It must expand the dollar character '$' and also translate any relative path.
 * Once all data is fetched to use open accordingly, redirections() records the
 * given file descriptor in the the appropriate int array which had been given
 * from parameter.
 * It modifies the address where the given pointer line_ptr points to the end
 * of the processed string.
*/
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

char			*expand_filename(t_list **env, char **line_ptr)
{
	char		*word_object;
	char		*filename;

	if (!(filename = ft_strnew(0)))
		return (NULL);
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
		if (!word_object)
			return (NULL);
		if (!(join_newstr(&filename, word_object)))
			return (NULL);
		free(word_object);
	}
	return (filename);
}

int				open_file(int open_code, int *fd_command, char *file)
{
	if (open_code == 3)
	{
		if (fd_command[0] != 0)
			close(fd_command[0]);
		fd_command[0] = open(file, O_RDONLY);
		return (1);
	}
	if (fd_command[1] != 1)
		close(fd_command[1]);
	if (open_code == 1)
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd_command[1] = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	return (1);
}

int				redirections(t_list **env, char **line_ptr, int *fd_command)
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
		return (0);
	if (!open_file(open_code, fd_command, file))
		return (0);
	free(file);
	return (1);
}

/*
	printf(LINE(SYMBOLIC));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	printf("filename : |%s|\n", file);
	printf("fd[0] :%d\tfd[1] :%d\n",fd_command[0], fd_command[1]);
*/
