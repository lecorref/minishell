#ifndef MINISHELL_H
# define MINISHELL_H
# include "struct.h"
# include "libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <errno.h>
# ifdef LINUX
#  include <linux/errno.h>
# endif
/* ------------------------------------------------------------------------- */

/*
 * Environment control
 */

/*
 * These defines shorten literals to make the code more understandable.
 * NAME is of type t_list
 */
# define ENV_KEY(NAME) ((t_env *)((NAME)->content))->key
# define ENV_VALUE(NAME) ((t_env *)((NAME)->content))->value

/*
 * Create and store environment in a linked list of t_env from envp variable.
 * All the links of the list are malloc() and should be free() once the program
 * exit. If the first arg was malloc(), it can be free() after.
 *
 * @args:
 *	char **envp a string table containing entries with the format KEY=VALUE
 * @return:
 *	t_list *: a pointer to the head of a list.
 */
t_list	*create_env_list(char **envp);


/*
 * Return a table similar to envp from a linked list of t_env.
 * The table is malloc() and should be free() after use.
 * The result should correspond to the return of the `env` builtin.
 *
 * @args:
 *	t_list *head: a pointer to the head of a linked list.
 * @return:
 *	char **: a string table containing entries with the format KEY=VALUE
 */
char	**env_list_to_tab(t_list *head);


/*
 * Create a new t_env from a string (format: KEY=VALUE)and push it on top of
 * the linked list.
 * The string is duplicated and can be free() after use. If the key already
 * exist its old value is replaced by the new one.
 *
 * @args:
 *	- t_list **head: a pointer to the first link of the list
 *	- char *var: a string with the format KEY=VALUE
 */
void	add_env_variable(t_list **head, char *var);


/*
 * Will find a value if the key is stored inside a linked_list of t_env.
 * The string is NOT duplicated and shouldn't be free() after use.
 *
 * @args:
 *	- t_list **head: a pointer to the first link of the list
 *	- char *key: a string that will be compared against t_env->key
 * @return:
 *	char *: a string that correspond to the key that was sent
 */
char	*find_env_value(t_list **head, char *key);


/*
 * Will find if a link from the env linked list contain a specific key,
 * delete this link and rebuild the list.
 *
 * @args:
 *	- t_list **head: a pointer to the first link of the list
 *	- char *key: a string that will be compared against t_env->key
 */
void	delete_env_variable(t_list **head, char *key);

/*
 * Free each string from the t_env structure then free the structure. The
 * parameters are made so that the function is usable with ft_lstdel.
 *
 * @args:
 *	- void *env: a pointer to the t_env struct to delete
 *	- size_t size: necessary for pointer to function.
 */
void	free_env(void *env, size_t size);

/* ------------------------------------------------------------------------- */

/*
 * Loop functions
 */

/*
 * This loop should only exit on ctrlD, sigkill and "exit"
 */
int		main_loop(t_list *env);

/*
 * This can be replaced by GNL
 */
int		get_line(char **buff);

/*
 * This function will create a list of t_command from the input line.
 */
t_list	tokenize_line(char *buff);
t_listjb	*tokenize_line_jb(char *line, t_list **env);
void	find_redirections(t_listjb **cmd, t_list **env,
		char *command_line, int *fd_command);
char	*skip_char(char *str, char c);

/*
 * This function will find if the command is a builtin and execute it, or
 * execute said command in execve
 */
void	execute_command(t_list **head ,t_command *command);

void	free_command_list(t_list **command);

/* ------------------------------------------------------------------------- */

/*
 * Builtins
 */

int		pwd_builtin(t_list **head, t_command *cmd);
int		cd_builtin(t_list **head, t_command *cmd);
int		exit_builtin(t_list **head, t_command *cmd);
int		echo_builtin(t_list **head, t_command *cmd);
int		export_builtin(t_list **head, t_command *cmd);
int		unset_builtin(t_list **head, t_command *cmd);
int		env_builtin(t_list **head, t_command *cmd);
int		executable_builtin(t_list **head, t_command *cmd);

/* ------------------------------------------------------------------------- */

/*
 * Lexer/parser function that will buid t_command structure
 */

/* ------------------------------------------------------------------------- */

/*
 * errors/signal handlins/exits functions
 */

void	error_msg_bash(t_command *cmd, int errnb, char *err_msg);
void	error_msg(t_command *cmd, int errnb, char *err_msg);
void	ctrl_back_slash_handler(int signal);
void	ctrl_back_slash_handler_quit(int signal);
void	ctrl_c_handler(int signal);
void	ctrl_d_handler(char *line);

/* ------------------------------------------------------------------------- */

#endif
