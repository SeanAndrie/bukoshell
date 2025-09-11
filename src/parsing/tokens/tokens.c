/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 00:59:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/11 18:53:40 by sgadinga         ###   ########.fr       */
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

unsigned int	create_token_mask(t_token *head)
{
	unsigned int	mask;

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

char	*tokens_to_str(t_token *head)
{
	char	*str;
	t_token *curr;
	size_t	spaces;
	size_t	total_size;

	spaces = 0;
	total_size = 0;
	curr = head;
	while (curr)
	{
		if (curr->next)
			spaces++;
		total_size += ft_strlen(curr->lexeme);
		curr = curr->next;
	}
	total_size += spaces;
	str = ft_calloc(total_size + 1, sizeof(char));
	if (!str)
		return (NULL);
	curr = head;
	while (curr)
	{
		ft_strlcat(str, curr->lexeme, total_size + 1);
		if (curr->next) 
			ft_strlcat(str, " ", total_size + 1);
		curr = curr->next;
	}
	return (str);
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
