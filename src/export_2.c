#include "../includes/minishell.h"

static char	**copy_env_helper(char **env, int size)
{
	int		i;
	char	**new_env;

	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**add_key_only(char **env, char *key)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = copy_env_helper(env, i + 1);
	if (!new_env)
		return (NULL);
	new_env[i] = ft_strdup(key);
	new_env[i + 1] = NULL;
	free_array(env);
	return (new_env);
}

static char	**add_variable(char **env, char *key, char *value)
{
	int		i;
	char	**new_env;
	char	*tmp;

	i = 0;
	while (env[i])
		i++;
	new_env = copy_env_helper(env, i + 1);
	if (!new_env)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	new_env[i] = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_env[i])
	{
		free_array(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	free_array(env);
	return (new_env);
}

void	add_or_update_env(t_prompt *prompt, char *cmd)
{
	char	**temp;

	temp = ft_split(cmd, '=');
	if (temp[1])
		prompt->env = add_variable(prompt->env, temp[0], temp[1]);
	else
		prompt->env = add_variable(prompt-> env, temp[0], "");
	free_array(temp);
}
