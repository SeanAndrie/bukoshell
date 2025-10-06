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

#include <boolean.h>
#include <libft.h>
#include <parsing/tokens.h>

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

