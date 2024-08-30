#include "../includes/minishell.h"

void	free_exit(t_prompt *prompt)
{
	free_data(prompt, false);
	exit(1);
}

int	dup_process(t_prompt *prompt, t_command *command,
	int fd_in, int end[2])
{
	if (command->prev)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0)
		{
			ft_putstr_fd("dup2 error: failed to redirect input\n", 2);
			return (1);
		}
	}
	close(end[0]);
	if (command->next)
	{
		if (dup2(end[1], STDOUT_FILENO) < 0)
		{
			ft_putstr_fd("dup2 error: failed to redirect output\n", 2);
			return (1);
		}
	}
	close(end[1]);
	if (command->prev)
		close(fd_in);
	execute_command(prompt, command);
	return (0);
}

int	check_fd_heredoc(t_prompt *prompt, t_command *cmd, int end[2])
{
	int	fd_in;

	if (prompt->heredoc->status)
	{
		close(end[0]);
		if (!cmd->heredoc_name)
		{
			ft_putstr_fd("-minishell: heredoc filename is not set\n", 2);
			return (-1);
		}
		fd_in = open(cmd->heredoc_name, O_RDONLY);
		if (fd_in == -1)
		{
			ft_putstr_fd("-minishell: failed to open heredoc file\n", 2);
			return (-1);
		}
	}
	else
		fd_in = end[0];
	return (fd_in);
}

static char	**get_path(t_prompt *prompt)
{
	char	**paths;
	char	*tmp_path;
	char	*path;
	int		i;

	i = -1;
	path = ms_getenv(prompt->env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	free(path);
	while (paths[++i])
	{
		tmp_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = tmp_path;
	}
	return (paths);
}

int	execute_system_command(t_prompt *prompt, t_command *command)
{
	int		i;
	char	*build_path;
	char	**path;

	if (!access(command->str[0], F_OK))
		execve(command->str[0], command->str, prompt->env);
	path = get_path(prompt);
	if (!path)
		return (print_command_error(command));
	i = -1;
	while (path[++i])
	{
		build_path = ft_strjoin(path[i], command->str[0]);
		if (!build_path)
		{
			free_array(path);
			return (print_command_error(command));
		}
		if (!access(build_path, F_OK))
			execve(build_path, command->str, prompt->env);
		free(build_path);
	}
	free_array(path);
	return (print_command_error(command));
}
