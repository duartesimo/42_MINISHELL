#include "../includes/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_separator(char *str)
{
	return (*str == ' ' || *str == '\t' || *str == '|'
		|| *str == '>' || *str == '<');
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	is_invalid_char(char c)
{
	return (c == '#' || c == '*' || c == '.' || c == '~' || c == '>'
		|| c == '\'' || c == ':' || c == ',' || c == '%' || c == '}'
		|| c == '@' || c == ' ' || c == ']' || c == '[' || c == '|'
		|| c == '=' || c == '<' || c == '!' || c == '?' || c == '^'
		|| c == '/' || c == '\"' || c == '&' || c == '+' || c == '{'
		|| c == '-');
}

int	if_digit(char *str, int i)
{
	return (str[i] == '$' && ft_isdigit(str[i + 1]));
}
