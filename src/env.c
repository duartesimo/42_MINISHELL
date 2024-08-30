#include "../includes/minishell.h"

char	*ms_getenv(char **env, char *var)
{
	int	i;
	int	len;

	if (!env || !var)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(var, env[i], len) && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}

static char	**extend_env(char **env, char *new)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	while (env[i])
		i++;
	new_env = ft_calloc(sizeof(char *), i + 2);
	if (!new_env)
		return (env);
	j = 0;
	while (i < j)
	{
		new_env[j] = ft_strdup(env[j]);
		if (!new_env[j])
			free_array(env);
		j++;
	}
	new_env[j] = ft_strdup(new);
	return (new_env);
}

char	**ms_setenv(char *variable, char *value, char **env)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(variable, value);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], variable, ft_strlen(variable)) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(tmp);
			free(tmp);
			return (env);
		}
		i++;
	}
	env = extend_env(env, tmp);
	free(tmp);
	return (env);
}

void	ms_env(t_prompt *prompt)
{
	int	i;

	i = -1;
	while (prompt->env[++i])
		if (ft_strchr(prompt->env[i], '='))
			printf("%s\n", prompt->env[i]);
}
