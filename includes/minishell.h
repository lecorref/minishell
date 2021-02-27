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
# include <dirent.h>
# ifdef LINUX
#  include <linux/errno.h>
# endif
/* ------------------------------------------------------------------------- */

/*
 * Just for printf debug
*/
#define LINE(NAME) "\n======================-"#NAME"-========================\n"
#define LINE2 "-----------------\n"
/*
** Macros
*/
# define MAX_FD 25

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/*
 * Environment control
 */

/*
 * These defines shorten literals to make the code more understandable.
 * NAME is of type t_list.
 */
# define ENV_KEY(NAME) ((t_env *)((NAME)->content))->key
# define ENV_VALUE(NAME) ((t_env *)((NAME)->content))->value

# define CMD(NAME) ((t_command *)((NAME)->content))->command
# define CMD_FD(NAME) ((t_command *)((NAME)->content))->fd

/* ------------------------------------------------------------------------- */

/*
 * These functions will create/add/find/delete/free a list of the t_env struct,
 * from the envp (char **ep) argument of the main.
*/

t_list		*create_env_list(char **envp);
char		**env_list_to_tab(t_list *head);
void		add_env_variable(t_list **head, char *var);
char		*find_env_value(t_list **head, char *key);
void		delete_env_variable(t_list **head, char *key);
void		free_env(void *env, size_t size);

/* ------------------------------------------------------------------------- */

/*
 * Loop functions
 */

int			main_loop(t_list *env);

/*
 * Lexer/parser function that will buid t_command structure
 * These functions will create a list of t_command from the input line.
*/
t_list		*tokenize_line_jb(char *line, t_list **env);
int			find_redirections(t_list **cmd, t_list **env, char *command_line,
				int *fd_command);
char		*skip_char(char *str, char c);
/*
 * tokenize split
*/
char		**split_with_exception(char *str, char c, char *exception_set);
char		**split_with_exception_v2(char *str, char c, char *exception_set);
char		*ghosting(char *str, char c, char *exception_set, int *error);
/*
 * tokenize quotes
*/
char		*double_quotes(t_list **env, char **line_ptr);
char		*simple_quotes(char **line_ptr);
char		*no_quotes(t_list **env, char **line_ptr);
char		*quotes(t_list **env, char **line_ptr);
int			redirections(t_list **env, char **line_ptr, int *fd_command);
char		*doll_expand(t_list **env, char **line_ptr);
/*
 * tokenize utils
*/
void		delete_remaining_char(char *str, char c);
char		*end_of_object(char *str);
int			is_symbol(int c);
int			is_symbol_v2(int c);
char		*skip_char(char *str, char c);
int			*init_fd();
/*
** clear lists
*/
void		clear_commandlist(void *content);
void		clear_envlist(void *content);
void		ft_array_string_del(char **array);
//void		free_command_list(t_list **cmd); is that one the clear_commandlist?

/*
 * This function will find if the command is a builtin and execute it, or
 * execute said command with execve.
 */
int			exec_t(t_list **env, t_command *cmd);//test purpose
void		execute_command(t_list **env, t_command *cmd);


/* ------------------------------------------------------------------------- */

/*
 * Builtins
 */

int			echo_builtin(t_command *cmd);
int			pwd_builtin(t_command *cmd);
int			exit_builtin(t_command *cmd);
int			cd_builtin(t_list **head, t_command *cmd);
int			export_builtin(t_list **head, t_command *cmd);
int			unset_builtin(t_list **head, t_command *cmd);
int			env_builtin(t_list **head, t_command *cmd);
int			executable_builtin(t_list **head, t_command *cmd);


/* ------------------------------------------------------------------------- */

/*
 * errors/signal handlins/exits functions
 */

//void		error_msg_bash(t_command *cmd, char *err_msg, char *arg);
void		error_msg(char *bash, t_command *cmd, char *arg, char *err_msg);
void		ctrl_back_slash_handler(int signal);
void		ctrl_back_slash_handler_quit(int signal);
void		ctrl_c_handler(int signal);
void		ctrl_d_handler(char *line);

/* ------------------------------------------------------------------------- */

#endif
