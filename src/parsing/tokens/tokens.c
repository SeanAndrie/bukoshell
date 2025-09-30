/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/01 00:43:03 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/lexer.h>
#include <parsing/tokens.h>
#include <parsing/valid.h>

t_bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
}

unsigned int	create_token_mask(t_token *head)
{
	unsigned int	mask;

	mask = 0;
	while (head)
	{
		mask |= head->type;
		head = head->next;
	}
	return (mask);
}

t_token	*copy_tokens(t_token *start, t_token *end)
{
	t_token	*copy;
	t_token	*curr;

	copy = NULL;
	curr = start;
	while (curr && curr != end)
	{
		if (!append_token(&copy, curr->lexeme, curr->type))
			return (free_tokens(&copy), NULL);
		curr = curr->next;
	}
	return (copy);
}

t_bool	normalize_tokens(t_map *map, t_token *head)
{
	parameter_expansion(map, head);
	if (!handle_concatenation(&head, TOKEN_WORD))
		return (FALSE);
	remove_tokens(&head, TOKEN_WHITESPACE);
	handle_arithmetic(&head);
	if (!validate_tokens(head))
		return (FALSE);
	return (TRUE);
}

t_token	*create_tokens(char *line)
{
	t_token_type	type;
	t_token			*head;
	char			*lexeme;

	head = NULL;
	while (*line)
	{
		if (*line == '$')
			lexeme = process_parameter(&line, &type);
		else if (ft_strchr(OPERATOR_TOKENS, *line))
			lexeme = process_operator(&line, &type);
		else if (ft_strchr(GROUP_TOKENS, *line))
			lexeme = process_grouping(&line, &type);
		else
			lexeme = process_word(&line, &type);
		if (!lexeme)
			return (free_tokens(&head), NULL);
		if (!append_token(&head, lexeme, type))
			return (free(lexeme), free_tokens(&head), NULL);
		free(lexeme);
	}
	return (head);
}
