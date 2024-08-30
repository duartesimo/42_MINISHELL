#include "../includes/minishell.h"

static void	handle_digit_or_quote(char **word, int *len, int *i, bool is_digit)
{
	int	k;

	if (is_digit)
		k = *i + 2;
	else
		k = *i + 1;
	if (is_digit && (k >= *len || (*word)[k] == '\0'))
	{
		(*word)[*i] = '\0';
		*len = *i;
	}
	else
	{
		while ((*word)[k] != '\0')
		{
			(*word)[*i] = (*word)[k];
			(*i)++;
			k++;
		}
		(*word)[*i] = '\0';
		*len = *i;
	}
	*i = -1;
}

static bool	process_dollar_sign(char *str, int i, bool sing_quotes)
{
	if (sing_quotes)
		return (false);
	if (ft_isdigit(str[i + 1]))
		return (false);
	if (str[i + 1] == '?')
		return (true);
	if (solo_dollar_sign(str))
		return (false);
	return (true);
}

static bool	has_expandable_vars(char *str)
{
	bool	sing_quotes;
	bool	doub_quotes;
	int		i;

	sing_quotes = false;
	doub_quotes = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && (i == 0 || str[i - 1] != '\\') && !doub_quotes)
			sing_quotes = !sing_quotes;
		else if (str[i] == '"' && (i == 0 || str[i - 1] != '\\')
			&& !sing_quotes)
			doub_quotes = !doub_quotes;
		else if (str[i] == '$')
			return (process_dollar_sign(str, i, sing_quotes));
		else if (((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>'
					&& str[i + 1] == '>')) && has_dollar_sign(str, i))
			return (false);
		i++;
	}
	return (true);
}

static void	process_char(t_processing *ctx)
{
	char	current_char;

	current_char = (*ctx->str)[ctx->index];
	if (current_char == '\'' && !ctx->in_double_quote)
		ctx->in_single_quote = !ctx->in_single_quote;
	else if (current_char == '"' && !ctx->in_single_quote)
		ctx->in_double_quote = !ctx->in_double_quote;
	if (current_char == '$' && !ctx->in_single_quote)
	{
		if ((*ctx->str)[ctx->index + 1] == '?'
			&& has_expandable_vars(*ctx->str + ctx->index))
			swap_exitmark(ctx->prompt, ctx->str, ctx->len, &ctx->index);
		else if (has_expandable_vars(*ctx->str + ctx->index)
			&& !solo_dollar_sign(*ctx->str + ctx->index))
			handle_expand(ctx->prompt, ctx->str, ctx->len, &ctx->index);
		else if (!has_expandable_vars(*ctx->str + ctx->index)
			&& isdigit((*ctx->str)[ctx->index + 1]))
			handle_digit_or_quote(ctx->str, ctx->len, &ctx->index, true);
		else if (is_quote((*ctx->str)[ctx->index + 1])
			&& ((*ctx->str)[ctx->index + 2]
			&& !is_quote((*ctx->str)[ctx->index + 2]))
			&& !doll_between_quotes(*ctx->str + ctx->index))
			handle_digit_or_quote(ctx->str, ctx->len, &ctx->index, false);
		*(ctx->len) = ft_strlen(*ctx->str);
	}
}

void	expand_env_vars(t_prompt *prompt, char **str)
{
	int				i;
	int				len;
	t_processing	ctx;

	ctx.str = str;
	ctx.len = &len;
	ctx.prompt = prompt;
	ctx.in_single_quote = false;
	ctx.in_double_quote = false;
	len = ft_strlen(*str);
	i = 0;
	while (i < len)
	{
		ctx.index = i;
		process_char(&ctx);
		i++;
	}
	if (has_expandable_vars(*str) && dollar_question(*str))
		prompt->exit_codes[prompt->flag3++] = 1;
}
