#include "../includes/struct.h"
#include "../includes/minishell.h"
#include "../libft/includes/libft.h"

void	free_env(void *env, size_t size)
{
	(void)size;
	free(((t_env *)env)->key);
	free(((t_env *)env)->value);
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
			free_env(env->next->content, sizeof(env));
			free(env->next);
			env->next = tmp;
			break;
		}
		env = env->next;
	}
	if (!ft_strcmp(key, ENV_KEY(*list)))
	{
		tmp = (*list)->next;
		free_env((*list)->content, sizeof(env));
		free(*list);
		*list = tmp;
	}
}
