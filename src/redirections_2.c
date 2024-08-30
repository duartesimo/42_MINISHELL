#include "../includes/minishell.h"

extern int	g_exitcode;

static int	print_redirection_error(t_lexer *lexer)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ",
		STDERR_FILENO);
	if (lexer)
	{
		if (lexer->token == PIPE)
			ft_putstr_fd("'|'\n", STDERR_FILENO);
		else if (lexer->token == GREAT)
			ft_putstr_fd("'>'\n", STDERR_FILENO);
		else if (lexer->token == LESS)
			ft_putstr_fd("'<'\n", STDERR_FILENO);
		else if (lexer->token == GREAT_GREAT)
			ft_putstr_fd("'>>'\n", STDERR_FILENO);
		else if (lexer->token == LESS_LESS)
			ft_putstr_fd("'<<'\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("'newline'\n", STDERR_FILENO);
	return (1);
}

static int	validate_tokens_helper(t_lexer *lexer)
{
	g_exitcode = 2;
	return (print_redirection_error(lexer));
}

int	validate_tokens(t_prompt *prompt)
{
	t_lexer	*lexer;

	lexer = prompt->lexer;
	while (lexer)
	{
		if (lexer->token == PIPE)
		{
			if (!lexer->next || lexer->next->token == PIPE || !lexer->prev)
				return (validate_tokens_helper(lexer));
		}
		else if (lexer->token == LESS || lexer->token == GREAT
			|| lexer->token == LESS_LESS || lexer->token == GREAT_GREAT)
		{
			if (!lexer->next || lexer->next->token != IDENTIFIER)
				return (validate_tokens_helper(lexer));
		}
		lexer = lexer->next;
	}
	return (0);
}
