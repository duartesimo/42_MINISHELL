#include "../includes/minishell.h"

static void	parse_tokens(t_prompt *prompt, t_command *command)
{
	int	num_nodes;

	num_nodes = count_lexer_nodes(prompt->lexer);
	if (!command->str)
	{
		command->str = ft_calloc(num_nodes + 1, sizeof(char *));
		if (!command->str)
			return ;
	}
	while (prompt->lexer)
	{
		if (is_redirection_or_pipe(prompt))
			return ;
		if (prompt->lexer->token == IDENTIFIER)
			process_identifier(prompt, command);
		prompt->lexer = prompt->lexer->next;
	}
}

static void	handle_redirections(t_prompt *prompt, t_command *command)
{
	t_lexer	*temp;

	if (prompt->lexer && is_redirection_token(prompt->lexer->token))
	{
		command->num_redirections++;
		if (!command->redirect)
			command->redirect = create_node(NULL, prompt->lexer->token);
		else
			add_node(&command->redirect,
				create_node(NULL, prompt->lexer->token));
		prompt->lexer = prompt->lexer->next;
		if (prompt->lexer && prompt->lexer->str)
		{
			temp = get_last_node(command->redirect);
			temp->str = ft_strdup(prompt->lexer->str);
			prompt->lexer = prompt->lexer->next;
		}
	}
}

static void	handle_pipe(t_prompt *prompt, t_command **command, int *num_pipes)
{
	t_command	*temp;

	prompt->lexer = prompt->lexer->next;
	(*num_pipes)--;
	prompt->flag4 = 0;
	temp = init_simple_cmds();
	(*command)->next = temp;
	temp->prev = *command;
	*command = temp;
}

static void	parse_simple_commands(t_prompt *prompt, int num_pipes)
{
	t_lexer		*temp;
	t_command	*command;

	temp = prompt->lexer;
	command = prompt->command;
	while (prompt->lexer && num_pipes >= 0)
	{
		if (prompt->lexer->token == PIPE)
			handle_pipe(prompt, &command, &num_pipes);
		handle_redirections(prompt, command);
		if (prompt->lexer && prompt->lexer->token == PIPE)
			continue ;
		parse_tokens(prompt, command);
	}
	prompt->lexer = temp;
}

void	parser(t_prompt *prompt)
{
	int		num_pipes;
	t_lexer	*temp;

	if (!prompt->lexer)
		return ;
	num_pipes = 0;
	temp = prompt->lexer;
	while (temp)
	{
		if (temp->token == PIPE)
			num_pipes++;
		temp = temp->next;
	}
	prompt->flag4 = 0;
	parse_simple_commands(prompt, num_pipes);
	if (!prompt->command)
		return ;
}
