#include "../includes/minishell.h"

extern int	g_exitcode;

static int	if_question_mark(void)
{
	char	*str;

	str = ft_itoa(g_exitcode);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free(str);
	return (127);
}

static void	execute_special_variable(t_prompt *prompt, t_command *command)
{
	if (prompt->exit_codes[get_exit_stat(prompt)] == 1)
		g_exitcode = if_question_mark();
	else
		g_exitcode = execute_system_command(prompt, command);
	prompt->exit_codes[get_exit_stat(prompt)] = 2;
}

static void	execute_builtin(t_prompt *prompt, t_command *command)
{
	g_exitcode = builtin(prompt, command);
	free_data(prompt, false);
	exit(g_exitcode);
}

int	execute_command(t_prompt *prompt, t_command *command)
{
	if (command->redirect && configure_redirections(command))
		free_exit(prompt);
	if (command->builtin)
		execute_builtin(prompt, command);
	else if (!ft_strncmp(command->str[0], "$?", 3))
		execute_special_variable(prompt, command);
	else if (command->str[0])
		g_exitcode = execute_system_command(prompt, command);
	free_data(prompt, false);
	exit(g_exitcode);
}

int	single_command(t_prompt *prompt, t_command *command)
{
	char	*cmd;
	int		pid;
	int		status;

	status = 0;
	cmd = command->builtin;
	prompt->command = single_command_heredoc(command);
	if (cmd && (!ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "export", 7)
			|| !ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "exit", 5)))
		return (g_exitcode = builtin(prompt, prompt->command), 0);
	process_heredoc(prompt, prompt->command);
	set_signal_handlers(SIG_PARENT);
	pid = fork();
	if (pid < 0)
		errors_exit(3);
	if (pid == 0)
	{
		execute_command(prompt, prompt->command);
		set_signal_handlers(SIG_CHILD);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exitcode = WEXITSTATUS(status);
	set_signal_handlers(SIG_DEFAULT);
	return (0);
}
