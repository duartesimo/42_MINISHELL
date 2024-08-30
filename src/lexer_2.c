#include "../includes/minishell.h"

void	delete_lexer(t_lexer **lexer)
{
	t_lexer	*head;
	t_lexer	*temp;

	head = *lexer;
	if (!head)
		return ;
	while (head)
	{
		free(head->str);
		temp = head->next;
		free(head);
		head = temp;
	}
	*lexer = NULL;
}

void	delete_quotes(char *str)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			c = str[i++];
			while (str[i] && str[i] != c)
				str[j++] = str[i++];
			if (str[i])
				i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}

bool	skip_quotes(char *line, int *i)
{
	char	quote;

	quote = line[*i];
	if (ft_strchr(line + *i + 1, quote))
	{
		(*i)++;
		while (line[*i] != quote)
			(*i)++;
		(*i)++;
		return (true);
	}
	return (false);
}

static int	add_identifier_helper(char *temp, int *i)
{
	while (temp[*i] && !is_separator(temp + *i))
	{
		if (is_quote(temp[*i]))
		{
			if (!skip_quotes(temp, i))
			{
				ft_putstr_fd("minishell: unexpected EOF while"
					"looking for matching `", 2);
				ft_putchar_fd(temp[*i], 2);
				ft_putstr_fd("\n", 2);
				return (1);
			}
		}
		else
			(*i)++;
	}
	return (0);
}

int	add_identifier(t_lexer **lexer, char **str, int i)
{
	char	*temp;
	char	*value;
	t_lexer	*cmd;

	cmd = NULL;
	temp = *str;
	if (add_identifier_helper(temp, &i))
		return (1);
	value = ft_substr(temp, 0, i);
	if (!value)
		return (1);
	delete_quotes(value);
	if (ft_strlen(value) == 0)
	{
		free(value);
		value = ft_strdup("");
		if (!value)
			return (1);
	}
	cmd = create_node(value, IDENTIFIER);
	if (!cmd)
		return (free(value), 1);
	*str += i;
	add_node(lexer, cmd);
	return (0);
}
