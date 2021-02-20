#include "minishell.h"
#define LINE(NAME) "\n======================-"#NAME"-========================\n"
#define LINE2 "-----------------\n"

/*
** double_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the double-quote.
** It has to expand the dollar symbol if it is met.
** It modify the address where the given pointer line_ptr points to the end of
** the processed string.
** It returns a malloc() address to a null-terminated string.
*/

char			*double_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*tmp;
	char		*final_str;
	char		*expanded;

	printf(LINE(DOUBLE_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	str = *line_ptr;
	str++;
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
	str++;
	tmp = ft_substr(*line_ptr, 0, (str - *line_ptr));
	join_newstr(&final_str, tmp);
	free(tmp);
	*line_ptr = str + 1;
	printf("line TX : |%s|\n", final_str);
	return (final_str);
}

/*
** simple_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or the simple-quote.
** It doesn"t have to expand dollar character.
** It modify the address where the given pointer line_ptr points to the end of
** the processed string.
** It returns a malloc() address to a null-terminated string.
*/

char			*simple_quotes(char **line_ptr)
{
	char		*word_object;
	char		*tmp;

	printf(LINE(SIMPLE_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
	tmp = *line_ptr;
	*line_ptr += 1;
	while (**line_ptr != '\'')
		*line_ptr += 1;
	*line_ptr += 1;
	if (!(word_object = ft_substr(tmp, 0, (*line_ptr - tmp))))
		return (NULL);
	printf("line TX : |%s|\n", word_object);
	return (word_object);
}

/*
** no_quotes() process the given line to make it compatible with args of
** execve()
** The termination character for this formated word-object is the end of string
** or one of the characters met in is_symbol().
** It has to expand the dollar symbol if it is met.
** It modify the address where the given pointer line_ptr points to the end of
** the processed string.
** It returns a malloc() address to a null-terminated string.
*/

char			*no_quotes(t_list **env, char **line_ptr)
{
	char		*str;
	char		*tmp;
	char		*final_str;
	char		*expanded;

	printf(LINE(NO_QUOTES));
	printf("line RX : |%s|\n", *line_ptr);
	printf(LINE2);
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
	printf("line TX : |%s|\n", final_str);
	return (final_str);
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
