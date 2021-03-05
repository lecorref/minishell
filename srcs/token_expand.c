#include "minishell.h"

char			*expand_doll_digit(char digit)
{
	char		*expanded;

	if (digit != '0')
		expanded = ft_strnew(0);
	else
		expanded = ft_strdup("minishell");
	return (expanded);
}

char			*expand_exit_status()
{
	char		*expanded;

	if (!(expanded = ft_itoa(g_exit_status)))
		return (NULL);
	return (expanded);
}

char			*check_special_value(char **line_ptr, char quote)
{
	char		*value;

	if (ft_isdigit(**line_ptr))
		value = expand_doll_digit(**line_ptr);
	else if (**line_ptr == '?')
		value = expand_exit_status();
	else if (!**line_ptr || is_symbol_doll(**line_ptr))
	{
		if (quote == 'n' && (**line_ptr == '\"' || **line_ptr == '\''))
			return (value = ft_strnew(0));
		else
			return (value = ft_strdup("$"));
	}
	else if (is_special_char(**line_ptr))
		value = ft_substr(*line_ptr - 1, 0, 2);
	else
		return (NULL);
	*line_ptr += 1;
	return (value);
}

char			*doll_expand(t_list **env, char **line_ptr, char quote)
{
	char		*doll;
	char		*str_env;
	char		*value;

	*line_ptr += 1;
	doll = *line_ptr;
	value = NULL;
	if ((value = check_special_value(line_ptr, quote)))
		return (value);
	while (**line_ptr && !is_symbol_doll(**line_ptr)
			&& authorized_char(**line_ptr))
		*line_ptr += 1;
	if (!(doll = ft_substr(doll, 0, (*line_ptr - doll))))
		return (NULL);
	if (!(str_env = find_env_value(env, doll)))
		if (!(value = ft_strnew(0)))
			return (NULL);
	if (!value)
		if (!(value = ft_strdup(str_env)))
			return (NULL);
	free(doll);
	return (value);
}
