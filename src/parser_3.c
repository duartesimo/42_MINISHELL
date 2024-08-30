#include "../includes/minishell.h"

static int	count_builtin_nodes(t_lexer *lexer)
{
	int	num_nodes;

	num_nodes = 0;
	while (lexer && lexer->str && is_builtin_command(lexer->str))
	{
		num_nodes++;
		lexer = lexer->next;
	}
	return (num_nodes);
}

static t_lexer	*skip_node(t_lexer *token)
{
	if (token && token->next)
	{
		if (token->next->next || token->next->token)
			token = token->next->next;
		else
			token = token->next;
	}
	return (token);
}

int	count_lexer_nodes(t_lexer *lexer)
{
	int		i;
	t_lexer	*temp;

	i = 0;
	temp = lexer;
	while (temp && (temp->str || temp->token))
	{
		if (temp->token == PIPE)
			return (i);
		if (temp->token != IDENTIFIER)
			temp = skip_node(temp);
		else if (is_builtin_command(temp->str))
		{
			temp = temp->next;
			i += count_builtin_nodes(temp);
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	return (i);
}
