#include "minishell.h"

/* Signal
**
** If the disposition is set to SIG_DFL, then the default action associated
** with the signal (see signal(7)) occurs.
**
**
** ctrl-C
** As ctrlC will just print ^C and go to the new line AT ANYTIME, we can handle
** it inside the loop for when:
** 1. a blocking command is typed, for example: grep h
** 2. if something is typed and ctrlD is typed before the return button, it
** writes ^C and goes to new line/promt.
** 3. for when command line is empty and no return button pressed (crtlC is
** pressed instead).
**
** ctrl-\
** Before the user input something, if ctrl\ is typed, nothing happpens inside
** the loop, if ctrl\ is typed, it has a different comportament).
** 1. if a blocking command is typed, ctrl\ kils the comman with a (core dumped)
** message and goes to a new line - i.e.: shows the prompt).
** 2. if something is typed and ctrl\ is typer before the return button, it
** does nothing.
*/

/* ctrl-D
** Before the user input something, if ctrlD is typed, the string "exit" is
** written after the prompt, then the shell closes, and $SHLVL environment
** variable is decreased after any input, if ctrlD is typed, it has a different
** comportament:
** 1. if a blocking command is typed, ctrlD stops the command and goes to a
** new line - i.e.: shows the prompt).
** 2. if something is typed and ctrlD is typed before the return button, it
** does nothing.
*/

/* This loop should only exit on ctrlD (sigquit) and "exit" builtin.
** It should be able to refuse to exit on Ctrl-d if needed (when something is
** typed and then the ctrlD is typed before the return button, it doesn't exit;
** otherwise it'll exit, either the command or the shell).
** gnl return:
** 1 = line read,
** 0 = EOF
** -1 = error
*/

void	print_tok(void *content)
{
	printf("unexpanded : |%s|\t\tfd0: %d\tfd1: %d\n",
			((t_command*)content)->unexpanded,
			((t_command*)content)->fd[0],
			((t_command*)content)->fd[1]);
}

int		execute_command(t_list **env, t_command *cmd)
{
	int	ret;

	print_cmd(cmd);//TEST - TO DELETE LATER
	if ((ret = is_builtin(cmd)))
		ret = execute_builtin(env, cmd, ret);
	else
		ret = execute_extern(env, cmd);
	close_fd(cmd->fd);
	return (ret);
}

int		executer(t_list **env, t_list *cmd)
{
	while (cmd)
	{
		expander(env, COMMAND(cmd));
		if (execute_command(env, COMMAND(cmd)) == -2)
			return (-2);
		cmd = cmd->next;
	}
	return (g_exit_status);
}

int		main_loop(t_list **env)
{
	t_list	*cmd;
	char	*line;
	int		ret_gnl;

	signal(SIGQUIT, ctrl_back_slash_handler);
	ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
	while ((ret_gnl = gnl_ctrld(0, &line)) > 0)
	{
		cmd = tokenizer(line);
		free(line);
		//ft_lstiter(cmd, &print_tok);
		if (executer(env, cmd) == -2)
			return (g_exit_status);
		if (g_line_eraser == 0)
			ft_putstr_fd("\033[1;32mminishell$\033[0m ", 1);
		ft_lstclear(&cmd, &clear_commandlist);
	}
	free(line);
	ft_lstclear(env, &clear_envlist);
	if (ret_gnl == -1)
		return (-1);
	return (g_exit_status);
}
