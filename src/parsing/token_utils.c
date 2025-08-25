/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/26 01:03:30 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

int	is_token_type(t_token_type type, int category_mask)
{
	return ((type & category_mask) == (unsigned int)category_mask);
}

t_token	*create_token(char *lexeme, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->lexeme = ft_strdup(lexeme);
	if (!token->lexeme)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, char *lexeme, t_token_type type)
{
	t_token	*token;
	t_token	*last_token;

	token = create_token(lexeme, type);
	if (!token)
		return ;
	if (!*head)
	{
		*head = token;
		return ;
	}
	last_token = *head;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = token;
}

void	free_tokens(t_token **head)
{
	t_token	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->lexeme);
		free(*head);
		*head = next;
	}
}
