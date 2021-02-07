#ifndef STRUCT_H
# define STRUCT_H

typedef struct	s_command
{
	char		*command;
	char		**args;
	int			fd[2];
}				t_command;


typedef struct	s_env
{
	char		*key;
	char		*value;
}				t_env;

#endif
