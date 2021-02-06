#ifndef MINISHELL_H
# define MINISHELL_H
# include "struct.h"
# include "../libft/includes/libft.h"

/*
 * Environment control
 */

# define ENV_KEY ((t_env *)(env->content))->key
# define ENV_VALUE ((t_env *)(env->content))->value
/*
 * Create and store environment in a linked list of t_env from envp variable.
 * The function should split on \n first, then split on '=';
 */
t_list  *create_env_list(char **envp);

/*
 * This function should not destroy the list
 */
char	**env_list_to_tab(t_list **env);

/*
 * This add a new env variable inside the list, or replace it if the key
 * already exist.
 */
void	add_env_variable(t_list **env, char *var);
/*
 * this will be necessary for echo
 */
char	*find_env_value(t_list **env, char *key);
/*
 * Basically unset
 */
void	delete_env_variable(t_list **env, char *key);

/* ------------------------------------------------------------------------- */

/*
 * Loop functions
 */

/*
 * This loop should only exit on ctrlD, sigkill and "exit"
 */
int     main_loop(t_list *env);

/*
 * This can be replaced by GNL
 */
int     get_line(char **buff);

/*
 * This function will create a list of t_command from the input line.
 */
t_list  tokenize_line(char *buff);

/*
 * This function will find if the command is a builtin and execute it, or
 * execute said command in execve
 */
void    execute_command(t_command *command, t_list **env);

void    free_command_list(t_list **command);

/* ------------------------------------------------------------------------- */

/*
 * Builtins (not realted to env)
 */

void    pwd_builtin(t_list **env, t_command *cmd);

void    cd_builtin(t_list **env, t_command *cmd);

void    exit_builtin(t_list **env, t_command *cmd);

void    echo_builtin(t_list **env, t_command *cmd);
void    export_builtin(t_list **env, t_command *cmd);
void    unset_builtin(t_list **env, t_command *cmd);
void    env_builtin(t_list **env, t_command *cmd);

/* ------------------------------------------------------------------------- */

/*
 * Lexer/parser function that will buid t_command structure
 */

#endif
