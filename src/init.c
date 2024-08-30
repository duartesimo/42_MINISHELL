#include "../includes/minishell.h"

void	set_env_vars(t_prompt *prompt, char **argv)
{
	char	*key;
	char	*value;
	char	*path;

	path = "/usr/local/sbin:/usr/local/bin:/usr/bin:/bin";
	key = ms_getenv(prompt->env, "PATH=");
	if (!key)
		prompt->env = ms_setenv("PATH=", path, prompt->env);
	free(key);
	key = ms_getenv(prompt->env, "SHLVL=");
	if (key && ft_atoi(key) > 0)
		value = ft_itoa(ft_atoi(key) + 1);
	else
		value = ft_strdup("1");
	free(key);
	prompt->env = ms_setenv("SHLVL=", value, prompt->env);
	free (value);
	key = ms_getenv(prompt->env, "_=");
	if (key)
	{
		prompt->env = ms_setenv("_=", argv[0], prompt->env);
		free(key);
	}
}

int	init_pid(t_prompt *prompt)
{
	int			process_count;
	t_command	*command;

	if (!prompt || !prompt->command)
		return (1);
	process_count = 0;
	command = prompt->command;
	while (command)
	{
		process_count++;
		command = command->next;
	}
	if (process_count == 0)
		return (1);
	prompt->pid = ft_calloc(process_count, sizeof(int));
	if (!prompt->pid)
		return (errors_exit(1));
	return (0);
}

void	init_flags(t_prompt *prompt)
{
	prompt->heredoc->err_num = 0;
	prompt->heredoc->status = false;
	prompt->interact = false;
	prompt->flag1 = 0;
	prompt->flag2 = 0;
	prompt->flag3 = 0;
	prompt->flag4 = 0;
	prompt->reset = false;
}

t_command	*init_simple_cmds(void)
{
	t_command	*cmds;

	cmds = malloc(sizeof(t_command));
	if (!cmds)
		return (NULL);
	cmds->str = NULL;
	cmds->builtin = NULL;
	cmds->num_redirections = 0;
	cmds->heredoc_name = NULL;
	cmds->redirect = NULL;
	cmds->next = NULL;
	cmds->prev = NULL;
	return (cmds);
}

t_prompt	*reset_init(t_prompt *prompt, char **argv, char **env)
{
	prompt->lexer = NULL;
	prompt->command = init_simple_cmds();
	prompt->heredoc = malloc(sizeof(t_heredoc));
	if (!prompt->heredoc)
		return (NULL);
	prompt->pid = NULL;
	prompt->exit_codes = NULL;
	init_flags(prompt);
	prompt->env = duplicate_array(env);
	set_env_vars(prompt, argv);
	return (prompt);
}
