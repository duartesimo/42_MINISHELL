#include "../includes/minishell.h"

static void	update_string_with_expansion(t_expansion *ctx, char *expanded)
{
	char	*new_str;
	size_t	before_len;
	size_t	after_len;
	size_t	expanded_len;

	before_len = ctx->start;
	after_len = ft_strlen(*(ctx->word) + ctx->end);
	expanded_len = ft_strlen(expanded);
	new_str = malloc(before_len + expanded_len + after_len + 1);
	if (new_str)
	{
		ft_memcpy(new_str, *(ctx->word), before_len);
		ft_memcpy(new_str + before_len, expanded, expanded_len);
		ft_memcpy(new_str + before_len + expanded_len,
			*(ctx->word) + ctx->end, after_len + 1);
		free(*(ctx->word));
		*(ctx->word) = new_str;
		*(ctx->len) = ft_strlen(new_str);
		*(ctx->index) = ctx->start + expanded_len - 1;
	}
}

static void	expand_and_replace_var(t_expansion *ctx)
{
	char	*substr;
	char	*expanded;

	substr = ft_strndup(*(ctx->word) + ctx->start, ctx->end - ctx->start);
	expanded = replace(ctx->prompt, substr);
	free(substr);
	if (expanded)
	{
		update_string_with_expansion(ctx, expanded);
		free(expanded);
	}
}

static void	remove_invalid_var(t_expansion *ctx)
{
	size_t	before_len;
	size_t	after_len;
	char	*new_str;

	before_len = ctx->start;
	after_len = ft_strlen(ctx->word[0] + ctx->end);
	new_str = malloc(before_len + after_len + 1);
	if (new_str)
	{
		ft_memcpy(new_str, ctx->word[0], before_len);
		ft_memcpy(new_str + before_len, ctx->word[0] + ctx->end, after_len + 1);
		free(*(ctx->word));
		*(ctx->word) = new_str;
		*(ctx->len) = ft_strlen(new_str);
	}
}

static bool	extract_and_validate_var(t_expansion *ctx)
{
	int		i;
	char	*name;

	i = ctx->start + 1;
	while (ctx->word[0][i] && (ft_isalnum(ctx->word[0][i])
		|| ctx->word[0][i] == '_'))
		i++;
	ctx->end = i;
	name = ft_strndup(ctx->word[0] + ctx->start + 1, ctx->end - ctx->start - 1);
	if (!is_valid_var(ctx->prompt, name))
	{
		free(name);
		return (false);
	}
	free(name);
	return (true);
}

void	handle_expand(t_prompt *prompt, char **word, int *len, int *i)
{
	t_expansion	ctx;

	ctx.word = word;
	ctx.len = len;
	ctx.index = i;
	ctx.prompt = prompt;
	ctx.start = *i;
	ctx.end = *i + 1;
	if (!extract_and_validate_var(&ctx))
		remove_invalid_var(&ctx);
	else
		expand_and_replace_var(&ctx);
}
