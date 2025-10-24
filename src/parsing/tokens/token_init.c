/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/30 00:56:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

t_bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
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
    token->expandable = FALSE;
	return (token);
}

void    append_token(t_token **head, t_token *token)
{
	t_token	*last;

	if (!token)
		return ;
	if (!*head)
	{
		*head = token;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = token;
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

t_bool is_expandable(t_token *token)
{
    if (!token || !token->lexeme)
        return (FALSE);
    if (ft_strchr(token->lexeme, '$'))
        return (TRUE);
    else if (ft_strchr(token->lexeme, '*'))
        return (TRUE);
    else if (ft_strchr(token->lexeme, '~'))
        return (TRUE);
    return (FALSE);
}

