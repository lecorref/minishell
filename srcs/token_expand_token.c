#include "minishell.h"

static int	remove_quotes(char **str)
{
	char	*sub;
	int		len;
	
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
** expand_token() is here to deal with tricky tricky expanded like :
** > "HELLO$USER"THIS'is'still"a file name"'which is'solongand$USELESS
**
** yep, the string above could be only one expanded.
*/
static int	join_newstr_v2(char **str, const char *src)
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

char		*expand_token(t_list **env, char **line_ptr)
{
	char	*word_object;
	char	*expanded;

	expanded = NULL;
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
			if (!(join_newstr_v2(&expanded, word_object)))
				return (NULL);
			free(word_object);
		}
	}
	return (expanded);
}
