#include "../includes/minishell.h"

static int	setup_outfile(t_lexer *redirection)
{
	int	fd;

	if (redirection->token == GREAT)
		fd = open(redirection->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirection->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (errors_exit(5));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (errors_exit(2));
	}
	close(fd);
	return (0);
}

static int	setup_infile(char *str)
{
	int	fd;

	if (access(str, F_OK) != 0)
	{
		return (1);
		close(0);
	}
	fd = open(str, O_RDONLY);
	if (fd < 0)
		return (errors_exit(6));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (errors_exit(2));
	}
	close(fd);
	return (0);
}

int	configure_redirections(t_command *command)
{
	int		ret;
	t_lexer	*temp;

	temp = command->redirect;
	ret = 0;
	while (temp)
	{
		if (temp->token == GREAT || temp->token == GREAT_GREAT)
		{
			if (setup_outfile(temp))
				ret = 1;
		}
		else if (temp->token == LESS)
		{
			if (setup_infile(temp->str))
				ret = 1;
		}
		else if (temp->token == LESS_LESS)
		{
			if (setup_infile(command->heredoc_name))
				ret = 1;
		}
		temp = temp->next;
	}
	return (ret);
}
