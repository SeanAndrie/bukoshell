/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/07 15:08:06 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <expand.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/lexer.h>
#include <parsing/tokens.h>
#include <parsing/valid.h>

static void	*free_helper(char *lexeme, t_token *head)
{
	if (lexeme)
		free(lexeme);
	if (head)
		free_tokens(&head);
	return (NULL);
}

t_token	*copy_tokens(t_token *start, t_token *end)
{
	t_token	*copy;
	t_token	*curr;
	t_token	*token;

	copy = NULL;
	curr = start;
	while (curr && curr != end)
	{
		token = create_token(curr->lexeme, curr->type);
		if (!token)
		{
			free_tokens(&copy);
			return (NULL);
		}
		append_token(&copy, token);
		curr = curr->next;
	}
	return (copy);
}

void	apply_expansions(t_token **head, t_map *map, t_bool heredoc)
{
	t_token	*curr;
	t_token	*next;

	if (!head || !*head || !map)
		return ;
	curr = *head;
	while (curr)
	{
		next = curr->next;
		if (is_token_type(curr->type, TOKEN_WORD) && ft_strchr(curr->lexeme,
				'$'))
			apply_param_expansion(curr, map, heredoc);
		if (ft_strchr(curr->lexeme, '*') && !heredoc)
			apply_wildcard_expansion(head, curr);
		curr = next;
	}
}

t_bool	normalize_tokens(t_token **head, t_map *map)
{
	if (!handle_concatenation(head, TOKEN_WORD))
		return (FALSE);
	remove_tokens(head, TOKEN_WHITESPACE);
	apply_expansions(head, map, FALSE);
	handle_arithmetic(head);
	return (TRUE);
}

t_token	*create_tokens(char *line, t_bool suppress, t_bool heredoc)
{
	t_token_type	type;
	t_token			*head;
	t_token			*token;
	char			*lexeme;

	if (!line)
		return (NULL);
	head = NULL;
	while (*line)
	{
		if (*line == '$' || ft_strchr(OPERATOR_TOKENS, *line))
			lexeme = process_operator(&line, &type);
		else if (ft_strchr(GROUP_TOKENS, *line))
			lexeme = process_grouping(&line, &type, suppress, heredoc);
		else
			lexeme = process_word(&line, &type);
		if (!lexeme)
			return (free_helper(lexeme, head));
		token = create_token(lexeme, type);
		if (!token)
			return (free_helper(lexeme, head));
		append_token(&head, token);
		free(lexeme);
	}
	return (head);
}
