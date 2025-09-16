/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/16 14:25:43 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/clean.h>
#include <parsing/lexer.h>
#include <parsing/tokens.h>

bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
}

bool	append_token_list(t_token **head, t_token *new_tokens)
{
	t_token *curr;

	if (!new_tokens)
		return (false);
	if (!*head)
	{
		*head = new_tokens;
		return (true);
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_tokens;
	return (true);
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
