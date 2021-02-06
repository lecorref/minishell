#include "../includes/struct.h"
#include "../includes/minishell.h"
#include "../libft/includes/libft.h"

void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

void    delete_env_variable(t_list **list, char *key)
{
	t_list	*env;
	t_list	*tmp;

	env = *list;
	while (env->next)
	{
		if (!ft_strcmp(key, ENV_KEY(env->next)))
		{
			tmp = env->next->next;
			free_env(env->next->content);
			free(env->next);
			env->next = tmp;
			break;
		}
		env = env->next;
	}
	if (!ft_strcmp(key, ENV_KEY(*list)))
	{
		tmp = (*list)->next;
		free_env((*list)->content);
		free(*list);
		*list = tmp;
	}
}

