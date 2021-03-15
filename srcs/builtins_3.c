#include "minishell.h"

static void	update_export_underscore(t_list **env, t_command *cmd)
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

static void	print_export(char **export_tab)
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

static int	export_builtin_arg(t_list **env, t_list **export, t_command *cmd)
{
	int		i;
	char	dest[2];

	i = 1;
	ft_memset(dest, '\0', 2);
	if (cmd->command[1][0] == '-')
	{
		error_msg("bash", cmd, ft_strncpy(dest, cmd->command[1], 2),
															"invalid option");
		printf("export: usage: export [-fn] [name[=value] ...] or export -p\n");
	}
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

static int	export_error(t_list **env, t_command *cmd, t_list **export)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = -1;
	while (cmd->command[1][i] && (i < ft_strclen(cmd->command[1], '=')))
	{
		if (cmd->command[1][i] == '_')
			i++;
		else if ((ret = ft_isalnum((char)cmd->command[1][i])) == 0)
			break ;
		else
			i++;
	}
	if ((ft_isdigit(cmd->command[1][0]) || ret == 0 ||
				cmd->command[1][0] == '=') && cmd->command[1][0] != '-')
	{
		error_msg("bash", cmd, cmd->command[1], "not a valid identifier");
		g_exit_status = 1;
		return (RT_FAIL);
	}
	else
		export_builtin_arg(env, export, cmd);
	return (0);
}

int			export_builtin(t_list **env, t_command *cmd, t_list **export)
{
	char	**export_tab;

	update_export_underscore(env, cmd);
	if (cmd->command[0] && !cmd->command[1])
	{
		if (!(export_tab = env_list_to_tab(*export)))
			return (RT_FAIL);
		print_export(export_tab);
		ft_freetab(export_tab);
	}
	if (cmd->command[0] && cmd->command[1])
	{
		if (export_error(env, cmd, export) == RT_FAIL)
			return (RT_FAIL);
	}
	g_exit_status = 0;
	return (RT_SUCCESS);
}
