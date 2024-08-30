#include "../includes/minishell.h"

t_lexer	*create_node(char *str, t_tokens token)
{
	t_lexer	*node;

	node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!node)
		return (NULL);
	node->str = str;
	node->token = token;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	add_node(t_lexer **lexer, t_lexer *node)
{
	t_lexer	*temp;

	if (!lexer || !node)
		return ;
	if (!*lexer)
	{
		*lexer = node;
		return ;
	}
	temp = *lexer;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
	node->prev = temp;
}

t_lexer	*get_last_node(t_lexer *node)
{
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}
