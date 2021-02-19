#include "../includes/minishell.h"

void	free_env(void *env, size_t size)
{
	(void)size;
	free(((t_env *)env)->key);
	free(((t_env *)env)->value);
	free(env);
}

void    delete_env_variable(t_list **env, char *key)
{
	t_list	*tmp_env;
	t_list	*tmp;

	tmp_env = *env;
	while (tmp_env->next)
	{
		if (!ft_strcmp(key, ENV_KEY(tmp_env->next)))
		{
			tmp = tmp_env->next->next;
			free_env(tmp_env->next->content, sizeof(env));
			free(tmp_env->next);
			tmp_env->next = tmp;
			break;
		}
		tmp_env = tmp_env->next;
	}
	if (!ft_strcmp(key, ENV_KEY(*env)))
	{
		tmp = (*env)->next;
		free_env((*env)->content, sizeof(env));
		free(*env);
		*env = tmp;
	}
}
