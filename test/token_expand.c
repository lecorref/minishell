#include "minishell.h"

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
