/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/11 18:09:17 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>
#include <debug.h>

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	handle_signals();
	shell->status = 0;
	shell->line = NULL;
	shell->head = NULL;
	shell->root = NULL;
	shell->prompt_mask = 0;
	ft_memset(&shell->lexer_ctx, 0, sizeof(t_lexer_ctx));
	return (shell);
}

void	get_lexer_context(t_lexer_ctx *ctx, t_token *head, bool reset)
{	
	t_token 		*curr;
	t_quote_state	*state;

	if (reset)
		ft_memset(ctx, 0, sizeof(*ctx));
	curr = head;
	state = &ctx->quote_state;
	while (curr)
	{
		if (is_token_type(curr->type, T_LPAREN))
			ctx->subsh_depth++;
		else if (is_token_type(curr->type, T_RPAREN))
			ctx->subsh_depth--;
		if (is_token_type(curr->type, T_SQUOTE) && !state->in_double)
			state->in_single ^= 1;
		else if (is_token_type(curr->type, T_DQUOTE) && !state->in_single)
			state->in_double ^= 1;
		if (is_token_type(curr->type, TOKEN_CTRL_OP) && !curr->next)
			ctx->dangling_op = true;
		else
			ctx->dangling_op = false;
		curr = curr->next;
	}
}

bool	parse_prompt(t_shell *shell)
{
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (false);
	if (!handle_prompt_continuation(&shell->head, &shell->lexer_ctx))
		return (false);
	remove_tokens(&shell->head, TOKEN_QUOTE);
	if (!handle_concatenation(&shell->head))
		return (false);
	remove_tokens(&shell->head, TOKEN_WHITESPACE);
	if (DEBUG_MODE)
		print_tokens(shell->head, true);

	/*
	**  Revise token validation to account for 
	** 	continuation-exclusive cases
	*/
	
	// shell->root = create_syntax_tree(shell->head, NULL);
	// if (!shell->root)
	// 	return (false);
	// if (DEBUG_MODE)
	// 	print_syntax_tree(shell->root);
	return (true);
}

int	start_shell(t_shell *shell)
{
	if (!parse_prompt(shell))
	{
		free_shell(shell, false);
		return (2);
	}
	free_shell(shell, false);
	return (shell->status);
}
