/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 12:49:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/tokens.h>
#include <stdbool.h>

static t_token	*create_token(char *lexeme, t_token_type type)
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

bool	append_token(t_token **head, char *lexeme, t_token_type type)
{
	t_token	*token;
	t_token	*last_token;

	token = create_token(lexeme, type);
	if (!token)
		return (false);
	if (!*head)
	{
		*head = token;
		return (true);
	}
	last_token = *head;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = token;
	return (true);
}

t_token	*concat_tokens(t_token *head, t_token_type concat_type)
{
	char	*concat;
	t_token	*token;
	t_token	*curr;
	size_t	size;

	size = 0;
	curr = head;
	while (curr)
	{
		size += ft_strlen(curr->lexeme);
		curr = curr->next;
	}
	concat = ft_calloc(size + 1, sizeof(char));
	if (!concat)
		return (NULL);
	curr = head;
	while (curr)
	{
		ft_strlcat(concat, curr->lexeme, size + 1);
		curr = curr->next;
	}
	token = create_token(concat, concat_type);
	if (!token)
		return (free(concat), NULL);
	return (free(concat), token);
}

t_token	*pop_token_type(t_token **head, t_token_type type_to_extract)
{
	t_token	*start;
	t_token	*end;

	start = *head;
	if (!start || !is_token_type(start->type, type_to_extract))
		return (NULL);
	end = start;
	while (end->next && is_token_type(end->next->type, type_to_extract))
		end = end->next;
	*head = end->next;
	end->next = NULL;
	return (start);
}

void	remove_tokens(t_token **head, t_token_type type_to_remove)
{
	t_token	**curr;
	t_token	*temp;

	curr = head;
	while (*curr)
	{
		if (is_token_type((*curr)->type, type_to_remove))
		{
			temp = *curr;
			*curr = (*curr)->next;
			temp->next = NULL;
			free(temp->lexeme);
			free(temp);
		}
		else
			curr = &(*curr)->next;
	}
}
