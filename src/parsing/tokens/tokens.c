/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 12:17:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <libft.h>
#include <parsing/parsing.h>

static void	*free_helper(char *lexeme, t_token *head)
{
	if (lexeme)
		free(lexeme);
	if (head)
		free_tokens(&head);
	return (NULL);
}

static void	mark_expandable_tokens(t_token *head)
{
	t_token	*prev;
	t_token	*curr;

	if (!head)
		return ;
	curr = head;
	prev = NULL;
	while (curr)
	{
		if (is_token_type(curr->type, TOKEN_WHITESPACE))
		{
			curr = curr->next;
			continue ;
		}
		if ((!prev || !is_token_type(prev->type, T_HEREDOC))
			&& is_expandable(curr))
			curr->expandable = TRUE;
		prev = curr;
		curr = curr->next;
	}
}

void	apply_expansions(t_token **head, t_map *map, t_bool heredoc)
{
	t_token	*curr;
	t_token	*next;

	mark_expandable_tokens(*head);
	curr = *head;
	while (curr)
	{
		next = curr->next;
		if (curr->lexeme && is_token_type(curr->type, TOKEN_WORD)
			&& ft_strchr(curr->lexeme, '$'))
			apply_param_expansion(curr, map, heredoc);
		if (curr->lexeme && ft_strchr(curr->lexeme, '*')
			&& !is_token_type(curr->type, TOKEN_QUOTE) && !heredoc)
		{
			if (apply_wildcard_expansion(head, curr))
			{
				curr = *head;
				continue ;
			}
		}
		if (curr->lexeme && curr->lexeme[0] == '~' && !is_token_type(curr->type,
				TOKEN_QUOTE) && !heredoc)
			apply_tilde_expansion(curr, map);
		curr = next;
	}
}

t_bool	normalize_tokens(t_token **head, t_map *map)
{
	if (!head || !*head)
		return (FALSE);
	apply_expansions(head, map, FALSE);
	if (!handle_concatenation(head, TOKEN_WORD))
		return (FALSE);
	remove_tokens(head, TOKEN_WHITESPACE);
	mark_group_tokens(head);
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
