/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/29 16:03:40 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

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

t_token	*concat_tokens(t_token **head)
{
	char	*concat;
	t_token *curr;
	size_t	size;

	size = 0;
	curr = *head;
	while (curr)
	{
		size += ft_strlen(curr->lexeme);
		curr = curr->next;
	}
	concat = ft_calloc(size + 1, sizeof(char));
	if (!concat)
		return (NULL);
	curr = *head;
	while (curr)
	{
		ft_strlcat(concat, curr->lexeme, size + 1);
		curr = curr->next;
	}
	return (create_token(concat, T_WORD));	
}

t_token	*pop_tokens(t_token **head, t_token_type type_to_extract)
{
	t_token	*curr;
	t_token *prev;
	t_token *end;

	curr = *head;
	prev = NULL;
	while (curr && !is_token_type(curr->type, type_to_extract))
	{
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return (NULL);
	end = curr;
	while (end->next && is_token_type(end->next->type, type_to_extract))
		end = end->next;
	if (prev)
		prev->next = end->next;
	else
		*head = end->next;
	end->next = NULL;
	return (curr);	
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
