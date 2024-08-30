#include "../includes/minishell.h"

int	g_exitcode;

int	main(int argc, char **argv, char **env)
{
	t_prompt	*prompt;

	if (argc > 1)
	{
		printf("Please don't use arguments!\n");
		exit(1);
	}
	prompt = init_prompt(argv, env);
	set_signal_handlers(SIG_DEFAULT);
	run_shell_loop(prompt, argv, env);
	return (0);
}
