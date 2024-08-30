#include "../includes/minishell.h"

extern int	g_exitcode;

bool	is_valid_var(t_prompt *prompt, const char *var_name)
{
	int		i;
	size_t	name_len;

	name_len = ft_strlen(var_name);
	i = 0;
	while (prompt->env[i])
	{
		if (ft_strncmp(prompt->env[i], var_name, name_len) == 0
			&& prompt->env[i][name_len] == '=')
			return (true);
		i++;
	}
	return (false);
}

static char	*create_str_2(t_prompt *prompt, char *str, char *var, char *word)
{
	size_t	final_length;
	char	*final_str;

	final_length = (ft_strlen(str) - ft_strlen(word)) + ft_strlen(var);
	final_str = ft_calloc(final_length + 1, sizeof(char));
	if (!final_str)
		return (NULL);
	prompt->flag1 = 0;
	prompt->flag2 = 0;
	while (str[prompt->flag1] != '$')
	{
		final_str[prompt->flag2++] = str[prompt->flag1++];
	}
	prompt->flag1--;
	ft_strlcat(final_str, var, final_length + 1);
	prompt->flag1 += ft_strlen(word) + 1;
	while (final_str[prompt->flag2])
		prompt->flag2++;
	while (str[prompt->flag1])
		final_str[prompt->flag2++] = str[prompt->flag1++];
	return (final_str);
}

static size_t	ft_strncpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	while (i < size)
		dst[i++] = '\0';
	return (i);
}

static char	*get_exit(char *str, int *i)
{
	char	*val;
	int		start;
	int		length;

	start = *i;
	while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '\"'
		&& str[*i] != '/' && !ft_isalnum(str[*i]))
	{
		if (str[*i] == '$' && *i > start)
			break ;
		(*i)++;
	}
	length = *i - start;
	val = ft_calloc(length + 1, sizeof(char));
	if (!val)
		return (NULL);
	ft_strncpy(val, str + start, length);
	val[length] = '\0';
	return (val);
}

char	*swap_exitmark(t_prompt *prompt, char **str, int *len, int *i)
{
	char	*word;
	char	*exit;
	char	*final_str;

	word = get_exit(*str, i);
	exit = ft_itoa(g_exitcode);
	final_str = create_str_2(prompt, *str, exit, word);
	if (final_str)
	{
		free(*str);
		*str = final_str;
		*i = -1;
		*len = ft_strlen(*str);
	}
	free(word);
	free(exit);
	return (*str);
}
