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

void		print_export_2(char **alpha_order, int len, int i)
{
	char	*key;

	key = ft_substr(alpha_order[i], 0, sizeof(char) * (len + 1));
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(key, 1);
	ft_putchar_fd('\"', 1);
	ft_putstr_fd(&alpha_order[i][len + 1], 1);
	ft_putstr_fd("\"\n", 1);
	free(key);
}

static void	print_export(char **export_tab)
{
	char	**alpha_order;
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
			print_export_2(alpha_order, len, i);
		else if (ft_strchr(&alpha_order[i][0], '=') == NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(alpha_order[i], 1);
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
}

int			export_builtin_arg(t_list **env, t_list **export, t_command *cmd)
{
	int		i;
	char	dest[1];

	i = 1;

	ft_memset(dest, '\0', 2);
	if (cmd->command[1][0] == '-')
	{
		error_msg("bash", cmd, ft_strncpy(dest, cmd->command[1], 2),
															"invalid option");
	//	printf("export: usage: export [-fn] [name[=value] ...] or export -p\n");
	}
	while (cmd->command[i])
	{
		g_exit_status = 0;
		if (cmd->command[i][0] == '-')
		{
			error_msg("bash", cmd, ft_strncpy(dest, &cmd->command[1][0], 2),
															"invalid option");
			g_exit_status = 2;
			break ;
		}
		if (ft_strchr(&cmd->command[i][0], '=') != NULL)
		{
			printf("\nthere is =\n");
			add_env_variable(env, &cmd->command[i][0]);
			add_env_variable(export, &cmd->command[i][0]);
		}
		else if (ft_strchr(&cmd->command[i][0], '=') == NULL)
		{
			printf("\nno  =\n");
			add_env_variable(export, &cmd->command[i][0]);
		}
		i++;
	}
	return (RT_SUCCESS);
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
		if (export_unset_error(env, cmd, export) == RT_FAIL)
			return (RT_FAIL);
	}
//	g_exit_status = 0;
	return (RT_SUCCESS);
}
