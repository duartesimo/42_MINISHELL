#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	search_equal_sign(t_prompt *prompt, t_command *cmds, int i)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	if (ft_strchr(cmds->str[i], '='))
	{
		while (cmds->str[i][k] && cmds->str[i][k] != '=')
			k++;
		while (prompt->env[j])
		{
			if (ft_strncmp(prompt->env[j], cmds->str[i], k) == 0)
				return (1);
			j++;
		}
		return (0);
	}
	return (2);
}

int	key_exists(t_prompt *prompt, t_command *cmds, int i)
{
	int		j;

	j = 0;
	while (prompt->env[j])
	{
		if (ft_strncmp(prompt->env[j], cmds->str[i],
				ft_strlen(cmds->str[i])) == 0)
			return (1);
		else
			j++;
	}
	return (0);
}

void	update_env_value(t_prompt *prompt, t_command *cmds, int i)
{
	int		k;
	char	**tmp;

	tmp = ft_split(cmds->str[i], '=');
	if (!tmp)
		return ;
	k = 0;
	while (prompt->env[k])
	{
		if (tmp[0]
			&& ft_strncmp(prompt->env[k], cmds->str[i], ft_strlen(tmp[0])) == 0)
		{
			free(prompt->env[k]);
			prompt->env[k] = ft_strdup(cmds->str[i]);
			free_array(tmp);
			break ;
		}
		else
			k++;
	}
	if (!prompt->env[k])
		free_array(tmp);
}
