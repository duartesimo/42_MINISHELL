#include "../includes/minishell.h"

extern int	g_exitcode;

static int	process_heredoc_helper(t_prompt *prompt, t_lexer *lexer,
			t_command *command, char *name)
{
	int		heredoc;
	bool	quotes;

	quotes = ((lexer->str[0] == '\''
				&& lexer->str[ft_strlen(lexer->str) - 1] == '\'')
			|| (lexer->str[0] == '\"'
				&& lexer->str[ft_strlen(lexer->str) - 1] == '\"'));
	remove_quotes(lexer->str, '\'');
	remove_quotes(lexer->str, '\"');
	heredoc = make_heredoc(prompt, lexer, quotes, name);
	if (command && command->builtin && ft_strncmp(command->builtin, "echo", 5))
		prompt->heredoc->status = true;
	return (heredoc);
}

static char	*create_tmp_filename(void)
{
	static int	i = 0;
	char		*file_name;
	char		*tmp;

	tmp = ft_itoa(i);
	if (!tmp)
		return (NULL);
	file_name = ft_strjoin(".tmp", tmp);
	free(tmp);
	if (!file_name)
		return (NULL);
	i++;
	return (file_name);
}

int	process_heredoc(t_prompt *prompt, t_command *command)
{
	t_lexer	*temp;

	temp = command->redirect;
	while (temp)
	{
		if (temp->token == LESS_LESS)
		{
			if (command->heredoc_name)
				free(command->heredoc_name);
			command->heredoc_name = create_tmp_filename();
			if (!command->heredoc_name
				|| process_heredoc_helper(prompt, temp, command,
					command->heredoc_name))
			{
				prompt->heredoc->err_num = 1;
				return (1);
			}
		}
		temp = temp->next;
	}
	return (0);
}

t_command	*single_command_heredoc(t_command *command)
{
	t_lexer	*temp;

	temp = command->redirect;
	while (command->redirect && command->redirect->next)
		command->redirect = command->redirect->next;
	command->redirect = temp;
	return (command);
}
