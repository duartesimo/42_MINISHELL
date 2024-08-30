#include "../includes/minishell.h"

extern int	g_exitcode;

static int	count_processes(t_command *command)
{
	int	count;

	count = 0;
	while (command)
	{
		count++;
		command = command->next;
	}
	return (count);
}

static void	pipewait(t_prompt *prompt, int *pid)
{
	int	i;
	int	status;
	int	num_pipes;

	num_pipes = count_processes(prompt->command);
	i = 0;
	while (i < num_pipes)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
	{
		prompt->heredoc->status = WEXITSTATUS(status);
		g_exitcode = WEXITSTATUS(status);
	}
}

static int	ft_fork(t_prompt *prompt, t_command *command, int fd_in, int end[2])
{
	static int	i;

	if (!i)
		i = 0;
	if (prompt->reset == true)
	{
		i = 0;
		prompt->reset = false;
	}
	set_signal_handlers(SIG_PARENT);
	prompt->pid[i] = fork();
	if (prompt->pid[i] < 0)
		return (errors_exit(3));
	if (prompt->pid[i] == 0)
	{
		set_signal_handlers(SIG_CHILD);
		if (dup_process(prompt, command, fd_in, end))
			return (errors_exit(2));
	}
	i++;
	return (0);
}

static int	execute_helper(t_prompt *prompt,
			t_command *cmd, int *fd_in, int end[2])
{
	if (cmd->next)
		pipe(end);
	if (process_heredoc(prompt, cmd))
		return (1);
	if (ft_fork(prompt, cmd, *fd_in, end))
		return (1);
	close(end[1]);
	if (cmd->prev)
		close(*fd_in);
	*fd_in = check_fd_heredoc(prompt, cmd, end);
	return (0);
}

int	execute(t_prompt *prompt)
{
	t_command	*cmd;
	int			fd_in;
	int			end[2];

	fd_in = STDIN_FILENO;
	cmd = prompt->command;
	if (!cmd->next)
		return (single_command(prompt, cmd));
	while (cmd)
	{
		if (execute_helper(prompt, cmd, &fd_in, end))
			return (1);
		cmd = cmd->next;
	}
	pipewait(prompt, prompt->pid);
	set_signal_handlers(SIG_DEFAULT);
	return (0);
}
