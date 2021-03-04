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
** Just for printf debug
*/
# define LINE(NAME) "\n======================-"#NAME"-========================\n"
# define LINE2 "-----------------\n"
/*
** Macros
*/
# define MAX_FD 25

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

/*
** builtin code magic numbers
*/

# define BT_ECHO 1
# define BT_PWD 2
# define BT_EXIT 3
# define BT_CD 4
# define BT_EXPORT 5
# define BT_UNSET 6
# define BT_ENV 7

/* 
** Environment control
** These defines shorten literals to make the code more understandable.
** NAME is of type t_list.
*/
# define ENV_KEY(NAME) ((t_env *)((NAME)->content))->key
# define ENV_VALUE(NAME) ((t_env *)((NAME)->content))->value

# define CMD(NAME) ((t_command *)((NAME)->content))->command
# define CMD_FD(NAME) ((t_command *)((NAME)->content))->fd

# define COMMAND(NAME) (t_command *)((NAME)->content)
/* ------------------------------------------------------------------------- */

int	g_line_eraser;
int	g_exit_status;

/*
** These functions will create/add/find/delete/free a list of the t_env struct,
** from the envp (char **ep) argument of the main.
*/
t_list		*create_env_list(char **envp);
char		**env_list_to_tab(t_list *head);
void		add_env_variable(t_list **head, char *var);
char		*find_env_value(t_list **head, char *key);
void		delete_env_variable(t_list **head, char *key);
void		free_env(void *env, size_t size);
int			update_underscore(t_list **env, char *path_cmd);
char		*last_arg(t_command *cmd);

/* ------------------------------------------------------------------------- */

/*
** Loop functions
*/
int			main_loop(t_list **env);

/*
** Lexer/parser function that will buid t_command structure
** These functions will create a list of t_command from the input line.
*/
t_list		*tokenize_line_jb(char *line, t_list **env);
int			find_redirections(t_list **cmd, t_list **env, char *command_line,
				t_command *i_command);
char		*skip_char(char *str, char c);

/*
** tokenize split
*/
char		**split_with_exception(char *str, char c, char *exception_set);
char		**split_with_exception_v2(char *str, char c, char *exception_set);
char		*ghosting(char *str, char c, char *exception_set, int *error);

/*
** tokenize quotes
*/
char		*double_quotes(t_list **env, char **line_ptr);
char		*simple_quotes(char **line_ptr);
char		*no_quotes(t_list **env, char **line_ptr);
char		*quotes(t_list **env, char **line_ptr);
int			redirections(t_list **env, char **line_ptr, t_command *i_command);
char		*doll_expand(t_list **env, char **line_ptr, char quote);

/*
** tokenize utils
*/
void		delete_remaining_char(char *str, char c);
char		*end_of_object(char *str);
int			is_symbol(int c);
int			is_symbol_doll(int c);
int			is_symbol_v2(int c);
int			authorized_char(int c);
char		*skip_char(char *str, char c);
t_command	*init_command();

void	print_cmd(t_command *cmd);/////////delete

/*
** clear lists
*/
void		clear_commandlist(void *content);
void		clear_envlist(void *content);
void		ft_array_string_del(char **array);
//void		free_command_list(t_list **cmd); is that one the clear_commandlist?

/* ------------------------------------------------------------------------- */

/*
** This function will find if the command is a builtin and execute it, or
** execute said command with execve.
*/
int			execute_command(t_list **env, t_command *cmd);
void		close_fd(int *fd);
void		dup_fd(int *fd);

//tests
int			exec_t(t_list **env, t_command *cmd, char **arr_env);
void		clean_fd_n_wait(int *fd, int cpid);
void		dup_it(int *fd);

/*
** Builtins
*/
int			echo_builtin(t_list **env, t_command *cmd);
int			pwd_builtin(t_list **env, t_command *cmd);

int			exit_builtin(t_command *cmd);
int			exit_arg(t_command *cmd, size_t i);

int			cd_builtin(t_list **env, t_command *cmd);
int			update_pwd(t_list **env);
char		*expand_tilde_and_exceptions(t_list **env, char *a, t_command *cmd);

int			export_builtin(t_list **env, t_command *cmd);

int			unset_builtin(t_list **env, t_command *cmd);
int			env_builtin(t_list **env, t_command *cmd);

/*
** Executable external functions
*/
int			execute_extern(t_list **env, t_command *cmd);
int			is_builtin(t_command *cmd);
int			execute_builtin(t_list **env, t_command *cmd, int builtin_code);
int			parent_process(pid_t pid, char *pathcmd, char **env_tab);
char		*path_to_executable(t_list **env, t_command *cmd);
char		*absolute_path(char *cmd, char *home_path);
char		*relative_path(t_command *cmd, char **split_path, char *pwd_path);
char		*add_path_to_cmd(char *abs_path, char *executable);
int			test_cmd(char *env_path, char *executable);

/* ------------------------------------------------------------------------- */

/*
** errors/signal handlins/exits functions
*/
void		error_msg(char *bash, t_command *cmd, char *arg, char *err_msg);
int			error_msg_2(char *bash, t_command *cmd, char *arg, char *err_msg);
void		ctrl_back_slash_handler(int signal);
void		ctrl_back_slash_handler_quit(int signal);
void		display_prompt(int sign);
void		set_line_eraser(int sign);

/* ------------------------------------------------------------------------- */

#endif
