#include "../includes/minishell.h"

static char	*get_env(t_prompt *prompt, char *val)
{
	int	i[3];

	if (!val)
		return (NULL);
	i[0] = -1;
	while (prompt->env[++i[0]])
	{
		if (!ft_strncmp(val, prompt->env[i[0]], key_len(prompt->env[i[0]])))
		{
			free(val);
			i[1] = 0;
			while (prompt->env[i[0]][i[1]] != '=')
				i[1]++;
			val = ft_calloc(ft_strlen(prompt->env[i[0]]) - i[1] + 1,
					sizeof(char));
			if (!val)
				return (NULL);
			i[2] = -1;
			while (prompt->env[i[0]][i[1]])
				val[++i[2]] = prompt->env[i[0]][++i[1]];
			return (val);
		}
	}
	free(val);
	return (NULL);
}

static const char	*ft_strstr(const char *str, const char *to_find)
{
	int	i;
	int	j;

	i = 0;
	if (to_find[0] == '\0')
		return (str);
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] != '\0' && str[i + j] == to_find[j])
		{
			if (to_find[j + 1] == '\0')
				return (&str[i]);
			++j;
		}
		++i;
	}
	return (0);
}

static char	*remove_var_from_str(const char *str, const char *word)
{
	char		*result;
	const char	*ptr;
	char		*write_ptr;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
	{
		free (result);
		return (NULL);
	}
	ptr = str;
	write_ptr = result;
	while (*ptr != '\0')
	{
		if (ft_strstr(ptr, word) == ptr)
			ptr += ft_strlen(word);
		else
		{
			*write_ptr = *ptr;
			write_ptr++;
			ptr++;
		}
	}
	*write_ptr = '\0';
	return (result);
}

static char	*get_var(char *str)
{
	char	*val;
	int		i;
	int		j;
	int		start;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	start = i;
	while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"'
		&& str[i] != '/')
	{
		if (str[i] == '$' && i > start)
			break ;
		i++;
	}
	val = ft_calloc(i - start + 1, sizeof(char));
	j = 0;
	while (start < i)
		val[j++] = str[start++];
	return (val);
}

char	*replace(t_prompt *prompt, char *str)
{
	char	*final_str;
	char	*var;
	char	*word;
	char	*port;

	port = get_var(str);
	word = get_word(str);
	if (!ft_strncmp(word, "?", 2))
		prompt->exit_codes[prompt->flag3++] = 1;
	var = get_env(prompt, get_word(str));
	if (!var)
	{
		free(word);
		final_str = remove_var_from_str(str, port);
		free(port);
		return (final_str);
	}
	else
	{
		final_str = create_str(prompt, str, var, word);
		free(word);
		free(var);
		free(port);
		return (final_str);
	}
}
