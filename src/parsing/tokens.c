/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 19:04:46 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
}

unsigned int create_token_mask(t_token *head)
{
	unsigned int mask;

	if (!head)
		return (0);
	mask = 0;
	while (head)
	{
		mask |= head->type;
		head = head->next;
	}
	return (mask);
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
	if (!handle_concatenation(&head))
		return (free_tokens(&head), NULL);
	remove_tokens(&head, T_WHITESPACE);
	return (head);
}
