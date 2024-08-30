#include "../includes/minishell.h"

extern int	g_exitcode;

void	default_sigint(int sig)
{
	(void)sig;
	g_exitcode = 130;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	hdsig(int sig)
{
	(void)sig;
	g_exitcode = 130;
	printf("> ^C\n");
	close(0);
}

void	sig_parent(int sig)
{
	if (sig == SIGINT)
		printf("\n");
	else if (sig == SIGQUIT)
		printf("Quit (core dumped)\n");
}

void	set_signal_handlers(int state)
{
	if (state == SIG_DEFAULT)
	{
		signal(SIGINT, default_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == SIG_HEREDOC)
	{
		signal(SIGINT, &hdsig);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == SIG_CHILD)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else if (state == SIG_PARENT)
	{
		signal(SIGINT, &sig_parent);
		signal(SIGQUIT, &sig_parent);
	}
}
