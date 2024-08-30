#include "../includes/minishell.h"

static void	update_env_var(char **env, const char *new)
{
	char	*tmp;

	tmp = ft_strdup(new);
	if (tmp)
	{
		free(*env);
		*env = tmp;
	}
}

void	add_path_env(t_prompt *prompt)
{
	int		i;
	char	*tmp;

	i = 0;
	while (prompt->env[i])
	{
		if (!ft_strncmp(prompt->env[i], "PWD=", ft_strlen("PWD=")))
		{
			tmp = ft_strjoin("PWD=", prompt->pwd);
			update_env_var(&prompt->env[i], tmp);
			free(tmp);
		}
		else if (!ft_strncmp(prompt->env[i], "OLDPWD=", ft_strlen("OLDPWD="))
			&& prompt->oldpwd)
		{
			tmp = ft_strjoin("OLDPWD=", prompt->oldpwd);
			update_env_var(&prompt->env[i], tmp);
			free(tmp);
		}
		i++;
	}
}

void	change_path(t_prompt *prompt)
{
	char	*tmp;

	tmp = ft_strdup(prompt->pwd);
	if (!tmp)
	{
		perror("ft_strdup");
		return ;
	}
	free(prompt->oldpwd);
	prompt->oldpwd = tmp;
	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		perror("getcwd");
		return ;
	}
	free(prompt->pwd);
	prompt->pwd = tmp;
}
