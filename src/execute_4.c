#include "../includes/minishell.h"

static char	*handle_error_cmd_helper(char *cmd, char *tmp, int *error)
{
	struct stat	st;
	char		*message;

	if ((cmd[0] == '/' || cmd[0] == '.')
		&& stat(tmp, &st) == 0 && (S_ISDIR(st.st_mode) | S_IXUSR))
	{
		message = ": Is directory";
		*error = 126;
	}
	else
	{
		message = ": command not found";
		*error = 127;
	}
	return (message);
}

int	print_command_error(t_command *command)
{
	int			error;
	char		*string;
	char		*tmp;
	char		*message;

	if (!command->str && !command->str[0])
		return (1);
	if (command->str[0][0])
		tmp = ft_strdup(command->str[0]);
	else
		tmp = ft_strdup("\'\'");
	message = handle_error_cmd_helper(command->str[0], tmp, &error);
	string = ft_strjoin(tmp, message);
	ft_putendl_fd(string, STDERR_FILENO);
	free(string);
	free(tmp);
	return (error);
}
