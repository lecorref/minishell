#include "minishell.h"

int				expand_doll_quote(t_list **env, char **str,
		char **final_str, char quote)
{
	char		*expanded;

	if (!(expanded = doll_expand(env, str, quote)))
		return (0);
	if (!(join_newstr(final_str, expanded)))
		return (0);
	free(expanded);
	return (1);
}

int				join_str_before(char **str, char **line_ptr, char **final_str)
{
	char		*tmp;

	if (!(tmp = ft_substr(*line_ptr, 0, (*str - *line_ptr))))
		return (0);
	if (!(join_newstr(final_str, tmp)))
		return (0);
	free(tmp);
	*line_ptr = *str;
	return (1);
}

/*
** double_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the double-quote.
** It has to expand the dollar symbol if it is met.
** It modifies the address where the given pointer line_ptr points to the end of
** the processed string, after the quote.
** It returns a malloc() address to a null-terminated string.
*/
char			*double_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*final_str;

	if (!(final_str = ft_strnew(0)))
		return (NULL);
	str = *line_ptr;
	str++;
	while (*str && *str !=  '\"')
	{
		if (*str == '$')
		{
			if (!(join_str_before(&str, line_ptr, &final_str)))
				return (NULL);
			if (!(expand_doll_quote(env, &str, &final_str, 'y')))
				return (NULL);
			*line_ptr = str;
			continue;
		}
		str++;
	}
	if (*str)
		str++;
	if (!(join_str_before(&str, line_ptr, &final_str)))
		return (NULL);
	return (final_str);
}

/*
** no_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or one of the characters met in is_symbol().
** It has to expand the dollar symbol if it is met.
** It modifies the address where the given pointer line_ptr points to, to  the
** end of the processed string (eol or is_symbol() char).
** It returns a malloc() address to a null-terminated string.
*/
char			*no_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*final_str;

	str = *line_ptr;
	if (!(final_str = ft_strnew(0)))
		return (NULL);
	while (*str && !is_symbol(*str))
	{
		if (*str == '$')
		{
			if (!(join_str_before(&str, line_ptr, &final_str)))
				return (NULL);
			if (!(expand_doll_quote(env, &str, &final_str, 'n')))
				return (NULL);
			*line_ptr = str;
			continue;
		}
		str++;
	}
	if (!(join_str_before(&str, line_ptr, &final_str)))
		return (NULL);
	return (final_str);
}

/*
** simple_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the simple-quote.
** It doesn"t have to expand dollar character.
** It modify the address where the given pointer line_ptr points to the end of
** the processed string, after the quote.
** It returns a malloc() address to a null-terminated string.
*/
char			*simple_quotes(char **line_ptr)
{
	char		*word_object;
	char		*tmp;

	tmp = *line_ptr;
	*line_ptr += 1;
	while (**line_ptr != '\'')
		*line_ptr += 1;
	*line_ptr += 1;
	if (!(word_object = ft_substr(tmp, 0, (*line_ptr - tmp))))
		return (NULL);
	return (word_object);
}

/*
	printf(LINE(DOUBLE_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	printf("line TX : |%s|\n", final_str);

	printf(LINE(NO_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	printf("line TX : |%s|\n", final_str);

	printf(LINE(SIMPLE_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	printf("line TX : |%s|\n", word_object);
*/
