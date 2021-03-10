#include "minishell.h"

void		update_export_underscore(t_list **env, t_command *cmd)
{
	char	*last;
	char	*needle;
	int		len;

	last = last_arg(cmd);
	if (!(needle = ft_strchr(last, '=')))
		len = ft_strlen(last);
	else
		len = needle - last;
	if (!(needle = ft_substr(last, 0, len)))
		return ;
	update_underscore(env, needle);
	free(needle);
}

void			print_export(char **export_tab)
{
	char	**alpha_order;
	char	*key;
	int		i;
	int		len;

	i = 0;
	len = 0;
	if (!(alpha_order = alpha_order_array(export_tab)))
		return ;
	while (alpha_order[i])
	{
		len = ft_strclen(&alpha_order[i][0], '=');
		if (ft_strchr(&alpha_order[i][0], '='))
		{
			key = ft_substr(alpha_order[i], 0, sizeof(char) * (len + 1));
			printf("declare -x ");
			printf("%s", key);
			printf("\"%s\"\n", &alpha_order[i][len + 1]);
			free(key);
		}
		else if (ft_strchr(&alpha_order[i][0], '=') == NULL)
			printf("declare -x %s\n", alpha_order[i]);
		i++;
	}
}

int			export_builtin_arg(t_list **env, t_list **export, t_command *cmd)
{
	int		i;

	i = 1;
	while (cmd->command[i])
	{
		if (ft_strchr(&cmd->command[i][0], '='))
		{
			add_env_variable(env, &cmd->command[i][0]);
			add_env_variable(export, &cmd->command[i][0]);
		}
		else if (ft_strchr(&cmd->command[i][0], '=') == NULL)
			add_env_variable(export, &cmd->command[i][0]);
		i++;
	}
	g_exit_status = 0;
	return (RT_SUCCESS);
}


int			export_builtin(t_list **env, t_command *cmd, t_list **export)
{
	char	**export_tab;
	int		i;

	i = 0;
	export_tab = env_list_to_tab(*export);
	update_export_underscore(env, cmd);
	if (cmd->command[0] && !cmd->command[1])
		print_export(export_tab);
	ft_freetab(export_tab);
	if (cmd->command[0] && cmd->command[1])
	{
		if (ft_isdigit(cmd->command[1][0]) ||
		(ft_strnstr(cmd->command[1], "-", ft_strclen(cmd->command[1], '='))))
		{
			error_msg("bash", cmd, cmd->command[1], "not a valid identifier");
			return (RT_FAIL);
		}
		else
			export_builtin_arg(env, export, cmd);
	}
	g_exit_status = 0;
	return (RT_SUCCESS);
}

int			unset_builtin(t_list **env, t_command *cmd, t_list **export)
{
	int		i;

	i = 0;
	update_underscore(env, last_arg(cmd));
	if (cmd->command[0] && cmd->command[1])
	{
		while (cmd->command[++i])
		{
			delete_env_variable(env, cmd->command[i]);
			delete_env_variable(export, cmd->command[i]);
		}
	}
	g_exit_status = 0;
	return (RT_SUCCESS);
}
