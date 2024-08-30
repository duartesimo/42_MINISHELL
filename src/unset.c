#include "../includes/minishell.h"

static char	**remove_env_var(char **env, char *var)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) != 0)
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_array(env);
	return (new_env);
}

static int	var_exists(t_prompt *prompt, char *str)
{
	int	i;

	i = -1;
	while (prompt->env[++i])
	{
		if (!ft_strncmp(prompt->env[i], str, ft_strlen(str)))
			return (1);
	}
	return (0);
}

static int	validate_param_unset(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || ft_isdigit(str[0]) || ft_strchr(str, '='))
	{
		ft_putstr_fd("unset: `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	while (str[i])
	{
		if (is_invalid_char(str[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(str, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ms_unset(t_prompt *prompt, t_command *command)
{
	int		i;

	if (!command->str[0])
		return (1);
	i = 0;
	while (command->str[i])
	{
		if (validate_param_unset(command->str[i]))
			break ;
		if (var_exists(prompt, command->str[i]))
			prompt->env = remove_env_var(prompt->env, command->str[i]);
		i++;
	}
	return (0);
}
