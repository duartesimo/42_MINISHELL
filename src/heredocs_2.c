#include "../includes/minishell.h"

extern int	g_exitcode;

static bool	stdin_state(void)
{
	int	test;

	test = dup(0);
	if (test == -1)
		return (false);
	close(test);
	return (true);
}

static int	read_hd_lines(t_prompt *prompt, t_lexer *lexer, bool quotes, int fd)
{
	char	*str;

	str = readline("> ");
	while (str && ft_strncmp(str, lexer->str, ft_strlen(lexer->str) + 1)
		&& g_exitcode != 130)
	{
		if (!quotes && !solo_dollar_sign(str)
			&& ft_strncmp(str, lexer->str, ft_strlen(lexer->str) + 1))
		{
			prompt->flag3 = 0;
			expand_env_vars(prompt, &str);
		}
		ft_putendl_fd(str, fd);
		free(str);
		str = readline("> ");
	}
	if (str)
		free(str);
	if (g_exitcode == 130)
		return (1);
	return (0);
}

int	make_heredoc(t_prompt *prompt, t_lexer *lexer,
	bool quotes, char *name)
{
	int		fd;
	int		result;
	int		stdinbackup;

	set_signal_handlers(SIG_HEREDOC);
	stdinbackup = dup(0);
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	result = read_hd_lines(prompt, lexer, quotes, fd);
	set_signal_handlers(SIG_DEFAULT);
	close(fd);
	if (g_exitcode == 130)
		remove(name);
	if (result == 1 || g_exitcode == 130)
	{
		g_exitcode = 0;
		if (!stdin_state())
		{
			dup2(stdinbackup, 0);
			close(stdinbackup);
			return (1);
		}
	}
	close(stdinbackup);
	return (0);
}
