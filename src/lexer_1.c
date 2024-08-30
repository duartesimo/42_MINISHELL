#include "../includes/minishell.h"

static int	add_separator_helper(t_lexer **lexer, t_tokens token, char **str)
{
	t_lexer	*cmd;

	cmd = create_node(NULL, token);
	if (!cmd)
		return (1);
	add_node(lexer, cmd);
	(*str)++;
	if (token == GREAT_GREAT || token == LESS_LESS)
		(*str)++;
	return (0);
}

static int	add_separator(t_lexer **lexer, char **str)
{
	int	ret;

	ret = 1;
	if (!ft_strncmp(*str, ">>", 2))
		ret = add_separator_helper(lexer, GREAT_GREAT, str);
	else if (!ft_strncmp(*str, "<<", 2))
		ret = add_separator_helper(lexer, LESS_LESS, str);
	else if (!ft_strncmp(*str, "|", 1))
		ret = add_separator_helper(lexer, PIPE, str);
	else if (!ft_strncmp(*str, ">", 1))
		ret = add_separator_helper(lexer, GREAT, str);
	else if (!ft_strncmp(*str, "<", 1))
		ret = add_separator_helper(lexer, LESS, str);
	return (ret);
}

static int	lexer_helper(char **str, t_lexer **token, char *input)
{
	int	fail;

	fail = 0;
	while (*str && **str)
	{
		if (fail)
		{
			delete_lexer(token);
			free(input);
			return (1);
		}
		while (ft_isspace(**str))
			(*str)++;
		if (!ft_strncmp(*str, "|", 1) || !ft_strncmp(*str, "<", 1)
			|| !ft_strncmp(*str, ">", 1))
			fail = add_separator(token, str);
		else
			fail = add_identifier(token, str, 0);
		while (ft_isspace(**str))
			(*str)++;
	}
	return (0);
}

t_lexer	*lexer(char *input)
{
	char	*str;
	t_lexer	*token;

	token = NULL;
	str = input;
	while (ft_isspace(*str))
		str++;
	if (*str == '\0')
	{
		free(input);
		return (NULL);
	}
	if (lexer_helper(&str, &token, input))
		return (NULL);
	free(input);
	return (token);
}
