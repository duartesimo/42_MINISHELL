#include "../includes/minishell.h"

int	key_len(char *str)
{
	int	i;

	i = -1;
	while (str[++i] && str[i] != '=')
		;
	return (i);
}

char	*create_str(t_prompt *prompt, char *str, char *var, char *word)
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
		final_str[prompt->flag2++] = str[prompt->flag1++];
	ft_strlcat(final_str, var, final_length + 1);
	prompt->flag1 += ft_strlen(word) + 1;
	while (final_str[prompt->flag2])
		prompt->flag2++;
	while (str[prompt->flag1])
		final_str[prompt->flag2++] = str[prompt->flag1++];
	return (final_str);
}

char	*get_word(char *str)
{
	char		*val;
	int			i;
	int			j;
	int			start;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	i++;
	start = i;
	while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"'
		&& str[i] != '/' && str[i] != '$')
		i++;
	val = ft_calloc(i - start + 1, sizeof(char));
	j = 0;
	while (start < i)
		val[j++] = str[start++];
	return (val);
}
