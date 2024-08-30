#include "../includes/minishell.h"

int	is_redirection_token(t_tokens token)
{
	return (token == LESS || token == GREAT
		|| token == LESS_LESS || token == GREAT_GREAT);
}

bool	is_redirection_or_pipe(t_prompt *prompt)
{
	if (is_redirection_token(prompt->lexer->token)
		|| prompt->lexer->token == PIPE)
		return (true);
	return (false);
}

void	process_identifier(t_prompt *prompt, t_command *command)
{
	if (prompt->flag4 == 0 && is_builtin_command(prompt->lexer->str)
		&& !command->builtin)
		command->builtin = ft_strdup(prompt->lexer->str);
	else
	{
		command->str[prompt->flag4] = ft_strdup(prompt->lexer->str);
		prompt->flag4++;
	}
}
