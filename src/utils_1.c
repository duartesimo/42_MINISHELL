#include "../includes/minishell.h"

char	*remove_quotes(char *str, char quote)
{
	int	i;
	int	j;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != quote)
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}

char	**duplicate_array(char **arr)
{
	int		i;
	int		count;
	char	**new;

	count = 0;
	while (arr && arr[count])
		count++;
	new = ft_calloc(count + 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new[i] = ft_strdup(arr[i]);
		if (!new[i])
		{
			free_array(new);
			return (NULL);
		}
		i++;
	}
	return (new);
}

char	*char_to_str(char c)
{
	char	*str;

	str = ft_calloc(2, sizeof(char));
	if (!str)
		return (NULL);
	str[0] = c;
	return (str);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	new_str = (char *)malloc(len + 1);
	if (new_str == NULL)
		return (NULL);
	ft_memcpy(new_str, s, len);
	new_str[len] = '\0';
	return (new_str);
}
