#include "../includes/minishell.h"

static int	change_directory(char *path)
{
	int		ret;

	ret = chdir(path);
	if (ret != 0)
	{
		ft_putstr_fd("-minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (ret);
}

static char	*find_path_ret(t_prompt *prompt, char *str)
{
	int		i;
	int		len;
	char	*new_str;

	i = 0;
	len = ft_strlen(str);
	while (prompt->env[i])
	{
		if (!ft_strncmp(prompt->env[i], str, len))
		{
			new_str = ft_substr(prompt->env[i], len,
					ft_strlen(prompt->env[i]) - len);
			if (new_str)
				return (new_str);
			else
				return (NULL);
		}
		i++;
	}
	return (NULL);
}

static void	print_oldpwd(t_prompt *prompt)
{
	char	*oldpwd;

	oldpwd = find_path_ret(prompt, "OLDPWD=");
	if (oldpwd)
	{
		printf("%s\n", oldpwd);
		free(oldpwd);
	}
}

static int	move_to_path(t_prompt *prompt, char *str)
{
	int		ret;
	char	*tmp;

	tmp = find_path_ret(prompt, str);
	if (!tmp)
	{
		ft_putstr_fd("-minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(" not set", STDERR_FILENO);
		return (1);
	}
	ret = change_directory(tmp);
	free(tmp);
	return (ret);
}

int	ms_cd(t_prompt *prompt, t_command *command)
{
	int		ret;

	if (!command->str[0])
		ret = move_to_path(prompt, "HOME=");
	else if (command->str[1] != NULL)
	{
		ft_putstr_fd("-minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (!ft_strncmp(command->str[0], "~", 1)
		|| !ft_strncmp(command->str[0], "--", 2))
		ret = move_to_path(prompt, "HOME=");
	else if (!ft_strncmp(command->str[0], "-", 1))
	{
		ret = move_to_path(prompt, "OLDPWD=");
		if (!ret)
			print_oldpwd(prompt);
	}
	else
		ret = change_directory(command->str[0]);
	if (ret != 0)
		return (1);
	change_path(prompt);
	add_path_env(prompt);
	return (ret);
}
