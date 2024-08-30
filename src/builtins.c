#include "../includes/minishell.h"

int	is_builtin_command(char *str)
{
	if (!str)
		return (0);
	return (!ft_strncmp(str, "echo", 5) || !ft_strncmp(str, "cd", 3)
		|| !ft_strncmp(str, "pwd", 4) || !ft_strncmp(str, "export", 7)
		|| !ft_strncmp(str, "unset", 6) || !ft_strncmp(str, "env", 4)
		|| !ft_strncmp(str, "exit", 5));
}

int	builtin(t_prompt *prompt, t_command *command)
{
	char	*cmd;
	int		ret;

	ret = 0;
	cmd = command->builtin;
	if (!ft_strncmp(cmd, "echo", 5))
		ms_echo(prompt, command);
	else if (!ft_strncmp(cmd, "cd", 3))
		ret = ms_cd(prompt, command);
	else if (!ft_strncmp(cmd, "pwd", 4))
		printf("%s\n", prompt->pwd);
	else if (!ft_strncmp(cmd, "export", 7))
		ret = ms_export(prompt, command);
	else if (!ft_strncmp(cmd, "unset", 6))
		ms_unset(prompt, command);
	else if (!ft_strncmp(cmd, "env", 4))
		ms_env(prompt);
	else if (!ft_strncmp(cmd, "exit", 5))
		ret = ms_exit(prompt, command);
	return (ret);
}
