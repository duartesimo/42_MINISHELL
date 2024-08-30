#include "../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

static void	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	if (!lexer)
		return ;
	while (lexer)
	{
		tmp = lexer;
		lexer = lexer->next;
		free(tmp->str);
		free(tmp);
	}
}

static void	free_parser(t_command *command)
{
	t_command	*temp;

	while (command)
	{
		if (command->builtin)
			free(command->builtin);
		if (command->str)
			free_array(command->str);
		if (command->redirect)
			free_lexer(command->redirect);
		if (command->heredoc_name)
		{
			unlink(command->heredoc_name);
			free(command->heredoc_name);
		}
		temp = command;
		command = command->next;
		free (temp);
	}
}

void	free_data(t_prompt *prompt, bool restart)
{
	if (!prompt)
		return ;
	free_lexer(prompt->lexer);
	free_array(prompt->env);
	free_parser(prompt->command);
	free(prompt->heredoc);
	free(prompt->pid);
	free(prompt->exit_codes);
	if (restart)
		prompt = NULL;
	else
	{
		free(prompt->pwd);
		free(prompt->oldpwd);
		free(prompt);
	}
}
