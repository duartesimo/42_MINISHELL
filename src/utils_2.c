#include "../includes/minishell.h"

bool	doll_between_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && str[i + 1] == '$' && str[i + 2] == '\'')
			|| (str[i] == '\"' && str[i + 1] == '$' && str[i + 2] == '\"'))
			return (true);
		i++;
	}
	return (false);
}

bool	solo_dollar_sign(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && (str[i + 1] == ' ' || str[i + 1] == '\0'
				|| str[i + 1] == '\'' || str[i + 1] == '\"'))
			return (true);
	}
	return (false);
}

bool	has_dollar_sign(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

int	dollar_sign(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i + 1);
		i++;
	}
	return (0);
}

int	equal_sign(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i + 1);
		i++;
	}
	return (0);
}
