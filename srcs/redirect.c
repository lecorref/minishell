#include "minishell.h"

static char	*strnew_cat(char *str1, char *str2, int len2)
{
	char	*ret;
	int		len1;

	if (str1 == NULL)
	{
		ret = ft_strnew(len2);
	}
	else
	{
		len1 = ft_strlen(str1);
		ret = ft_strnew(len1 + len2);
		ret = ft_strncat(ret, str1, len1);
		free(str1);
	}
	return (ft_strncat(ret, str2, len2));
}

static char	*fill_cmd_fd(char *line, int *fd, int detect)
{
	int		append;
	char	*ptr;
	char	*filename;

	append = O_TRUNC;
	if ( detect > 1 && *(line + 1) == '>')
	{
		append = O_APPEND;
		line++;
	}
	line++;
	while (*line == ' ')
		line++;
	ptr = line;
	while (ft_isalnum(*line)) //not the right function, it should include .,/ and other
		line++;
	if (fd[detect - 1] != (detect - 1))
		close(fd[detect - 1]);
	filename = ft_strnew(line - ptr + 1);
	ft_strncpy(filename, ptr, line - ptr);
	fd[detect - 1] = open(filename, O_CREAT | O_RDWR | append, 0666);
	printf("%s\n", strerror(errno));
	free(filename);
	return (line);
}

static int	detect_sign(char *ptr, char *line)
{
	int		ret;

	ret = 0;
	if (*line == '>')
		ret = 2;
	else if (*line == '<')
		ret = 1;
	if (ret != 0 && line == ptr)
		return (-1);
	else if (ret == 2 && *(line - 1) == '2')
		return (3);
	return (ret);

}


int			check_redirect(char *line, t_list **head, int *fd)
{
	char	*ptr;
	int		detect;
	char	*cmd;

	ptr = line;
	detect = 0;
	cmd = NULL;
	while (*line != '\0')
	{
		detect = detect_sign(ptr, line);
		if (detect > 0)
		{
			cmd = strnew_cat(cmd, ptr, (line - 1 - ptr));
			line = fill_cmd_fd(line, fd, detect);
			ptr = line;
			detect = 0;
		}
		else if (detect < 0)
			return (-1);
		line++;
	}
	cmd = strnew_cat(cmd, ptr, (line - ptr));
	add_cmd_to_list(cmd, head, fd);
	return (0);
}
