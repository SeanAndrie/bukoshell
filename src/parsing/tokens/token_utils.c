/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/23 12:25:38 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <libft.h>
#include <parsing/tokens.h>

unsigned int	create_token_mask(t_token *head)
{
	unsigned int	mask;

	mask = 0;
	if (!head)
		return (0);
	while (head)
	{
		mask |= head->type;
		head = head->next;
	}
	return (mask);
}

static size_t	concat_size(t_token *head)
{
	size_t	size;

	if (!head)
		return (0);
	size = 0;
	while (head)
	{
		if (head->lexeme)
			size += ft_strlen(head->lexeme);
		head = head->next;
	}
	return (size);
}

t_token	*concat_tokens(t_token *head, t_token_type concat_type)
{
	char	*concat;
	t_token	*token;
	t_token	*curr;
	size_t	size;

	if (!head)
		return (NULL);
	size = concat_size(head);
	concat = ft_calloc(size + 1, sizeof(char));
	if (!concat)
		return (NULL);
	curr = head;
	while (curr)
	{
		if (curr->lexeme)
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

	if (!*head)
		return ;
	curr = head;
	while (*curr)
	{
		if (is_token_type((*curr)->type, type_to_remove) || !(*curr)->lexeme)
		{
			temp = *curr;
			*curr = (*curr)->next;
			if (temp->lexeme)
			{
				free(temp->lexeme);
				temp->lexeme = NULL;
			}
			free(temp);
		}
		else
			curr = &(*curr)->next;
	}
}
