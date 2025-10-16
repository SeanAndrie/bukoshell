/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:28:51 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 00:18:31 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <expand.h>
#include <boolean.h>
#include <parsing/valid.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

void	handle_arithmetic(t_token **head)
{
	t_token	*curr;

	curr = *head;
	while (curr)
	{
		if (curr->next && is_token_type(curr->type, TOKEN_GROUP)
			&& is_token_type(curr->next->type, TOKEN_GROUP))
		{
			curr->type |= TOKEN_ARITH;
			curr->next->type |= TOKEN_ARITH;
			curr = curr->next->next;
			continue ;
		}
		if (is_token_type(curr->type, TOKEN_GROUP) && !is_token_type(curr->type,
				TOKEN_ARITH))
			curr->type |= TOKEN_SUBSHELL;
		curr = curr->next;
	}
}

t_bool	handle_concatenation(t_token **head, t_token_type concat_type)
{
	t_token	**curr;
	t_token	*popped;
	t_token	*concat;

	curr = head;
	while (*curr)
	{
		if ((*curr)->next && is_token_type((*curr)->type, concat_type))
		{
			if (is_token_type((*curr)->next->type, concat_type))
			{
				popped = pop_token_type(curr, concat_type);
				if (!popped)
					return (FALSE);
				concat = concat_tokens(popped, concat_type);
				free_tokens(&popped);
				if (!concat)
					return (FALSE);
				concat->next = *curr;
				*curr = concat;
			}
		}
		curr = &(*curr)->next;
	}
	return (TRUE);
}

char *process_quotes_heredoc(char **line_ptr, t_token_type *type)
{
    char    quote;
    char    *lexeme;

    quote = **line_ptr;
    if (quote == '\'')
        *type = T_WORD_SQUOTE;
    else
        *type = T_WORD_DQUOTE;
    lexeme = ft_calloc(2, sizeof(char));
    if (!lexeme)
        return NULL;
    lexeme[0] = quote;
    (*line_ptr)++;
    return (lexeme);
}

t_token_type	categorize_ctrl_op(char c, t_bool is_double)
{
	if (c == '|')
	{
		if (is_double)
			return (T_OR);
		return (T_PIPE);
	}
	else if (c == '&')
	{
		if (is_double)
			return (T_AND);
		return (T_BACKGROUND);
	}
	return (TOKEN_NONE);
}

t_token_type	categorize_redirection(char c, t_bool is_double)
{
	if (c == '<')
	{
		if (is_double)
			return (T_HEREDOC);
		return (T_REDIR_IN);
	}
	else if (c == '>')
	{
		if (is_double)
			return (T_REDIR_APPEND);
		return (T_REDIR_OUT);
	}
	return (TOKEN_NONE);
}
