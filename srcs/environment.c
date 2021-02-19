#include "../includes/minishell.h"

static int	create_env_struct(char *keyvalue, t_env *env)
{
	char	*needle;

	if (!(needle = ft_strrchr(keyvalue, '=')))
		return (0);
	env->key = ft_substr(keyvalue, 0, needle - keyvalue);
	env->value = ft_strdup(needle + 1);
	return (1);
}

t_list  *create_env_list(char **envp)
{
	t_env	*new;
	t_list	*list;
	t_list	*tmp;

	new = (t_env*)malloc(sizeof(t_env));
	list = NULL;
	while (*envp != NULL)
	{
		create_env_struct(*envp, new);
		tmp = ft_lstnew(new);
		ft_lstadd_front(&list, tmp);
		envp++;
	}
	free(new);
	return (list);
}

char    **env_list_to_tab(t_list *env)
{
	int		count;
	t_list	*tmp_env;
	char	**new_env;
	char	*tmp_str;

	count = 1;
	tmp_env = env;
	while ((tmp_env = tmp_env->next))
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	ft_bzero(new_env, count + 1);
	while (--count >= 0 && env != NULL)
	{
		tmp_str = ft_strjoin(ENV_KEY(env), "=");
		new_env[count] = ft_strjoin(tmp_str, ENV_VALUE(env));
		free(tmp_str);
		env = env->next;
	}
	return new_env;
}

void    add_env_variable(t_list **list, char *var)
{
	t_env	*new;
	t_list	*env;

	new = (t_env*)malloc(sizeof(t_env));
	if (!create_env_struct(var, new))
			return ;
	env = *list;
	while (env)
	{
		if (!ft_strcmp(new->key, ENV_KEY(env)))
		{
			free(new->key);
			free(ENV_VALUE(env));
			ENV_VALUE(env) = new->value;
			break;
		}
		env = env->next;
	}
	if (env == NULL)
	{
		env = ft_lstnew(new);
		ft_lstadd_front(list, env);
	}
	free(new);
}

char	*find_env_value(t_list **list, char *key)
{
	t_list	*env;

	env = *list;
	while (env)
	{
		if (!ft_strcmp(key, ENV_KEY(env)))
			return (ENV_VALUE(env));
		env = env->next;
	}
	return (NULL);
}
