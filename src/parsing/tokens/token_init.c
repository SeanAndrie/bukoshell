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

t_bool	append_token(t_token **head, char *lexeme, t_token_type type)
{
	t_token	*token;
	t_token	*last_token;

	token = create_token(lexeme, type);
	if (!token)
		return (FALSE);
	if (!*head)
	{
		*head = token;
		return (TRUE);
	}
	last_token = *head;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = token;
	return (TRUE);
}
