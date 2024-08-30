#include "../includes/minishell.h"

static int	validate_param_export(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '=' || ft_isdigit(str[0]) || !str[0])
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	while (str[i] && str[i] != '=')
	{
		if (is_invalid_char(str[i]))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(str, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	print_sorted_env(char **var)
{
	int	i;
	int	j;

	i = 0;
	while (var[i])
	{
		j = 0;
		printf("declare -x ");
		while (var[i][j] && var[i][j] != '=')
		{
			printf("%c", var[i][j]);
			j++;
		}
		if (var[i][j] == '=')
			printf("=\"%s\"", var[i] + j + 1);
		printf("\n");
		i++;
	}
}

static void	swap_strs(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static char	**sort_env_variables(t_prompt *prompt)
{
	int		i;
	int		j;

	i = 0;
	while (prompt->env[i])
	{
		j = i + 1;
		while (prompt->env[j])
		{
			if (ft_strcmp(prompt->env[i], prompt->env[j]) > 0)
				swap_strs(&prompt->env[i], &prompt->env[j]);
			j++;
		}
		i++;
	}
	return (prompt->env);
}

int	ms_export(t_prompt *prompt, t_command *command)
{
	int		i;
	char	**var;

	i = -1;
	var = sort_env_variables(prompt);
	if (!command->str[0])
		print_sorted_env(var);
	else
	{
		while (command->str[++i])
		{
			if (validate_param_export(command->str[i]))
				return (1);
			if (search_equal_sign(prompt, command, i) == 0)
				add_or_update_env(prompt, command->str[i]);
			else if (search_equal_sign(prompt, command, i) == 1)
				update_env_value(prompt, command, i);
			else if (key_exists(prompt, command, i) == 1)
				continue ;
			else
				prompt->env = add_key_only(prompt->env, command->str[i]);
		}
	}
	return (0);
}
