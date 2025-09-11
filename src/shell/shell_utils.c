/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:35:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/11 18:30:59 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static bool	incomplete_prompt(t_lexer_ctx *ctx)
{
	return (ctx->subsh_depth > 0 || ctx->quote_state.in_single 
			|| ctx->quote_state.in_double || ctx->dangling_op);
}

void	append_newline(t_token *head)
{
	t_token *curr;
	char	*temp;

	curr = head;
	while (curr)
	{
		if (is_token_type(curr->type, TOKEN_WORD))
		{
			temp = ft_strjoin("\n", curr->lexeme);
			free(curr->lexeme);
			curr->lexeme = temp;
		}
		curr = curr->next;
	}
}

bool	handle_prompt_continuation(t_token **head, t_lexer_ctx *ctx)
{
	t_token	*new;
	char	*line;
	char	*token_str;

	get_lexer_context(ctx, *head, true);
	while (incomplete_prompt(ctx))
	{
		line = readline(PS2); 
		if (!line)
			return (false);
		new = create_tokens(line);
		free(line);
		if (!new)
			return (false);
		if (ctx->quote_state.in_single || ctx->quote_state.in_double)
			append_newline(new);
		if (!append_token_list(head, new))
			return (free_tokens(&new), false);
		get_lexer_context(ctx, new, false);
		token_str = tokens_to_str(*head);
		add_history(token_str);
	}
	return (true);
}
