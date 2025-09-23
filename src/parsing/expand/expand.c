/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 01:49:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 17:27:03 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <expand.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

bool	handle_expansion(t_map *map, t_token *token)
{
	char		*key;
	char		*temp;
	t_environ	*entry;

	temp = ft_strdup(token->lexeme);
	if (!temp)
		return (false);
	key = ft_substr(token->lexeme, 1, ft_strlen(token->lexeme) - 1);
	if (!key)
		return (free(temp), false);
	entry = search_entry(map, key);
	free(key);
	if (!entry)
		return (free(temp), false);
	free(token->lexeme);
	token->lexeme = ft_strdup(entry->value);
	if (!token->lexeme)
		return (token->lexeme = temp, false);
	free(temp);
	return (true);
}

char	*create_expanded_lexeme(t_map *map, t_token *tokens)
{
	t_token	*curr;
	t_token	*concat;
	char	*lexeme;

	curr = tokens;
	while (curr)
	{
		handle_expansion(map, curr);
		curr = curr->next;
	}
	concat = concat_tokens(tokens, T_PARAMETER);
	if (!concat)
		return (NULL);
	lexeme = ft_strdup(concat->lexeme);
	free_tokens(&concat);
	if (!lexeme)
		return (NULL);
	return (lexeme);
}

static void	apply_expansion(t_map *map, t_token *head)
{
	t_token	*tokens;
	char	*stripped;

	if (is_token_type(head->type, T_WORD_SQUOTE))
		return ;
	stripped = ft_strtrim(head->lexeme, "\"");
	if (!stripped)
		return ;
	tokens = create_tokens(stripped);
	free(stripped);
	if (!tokens)
		return ;
	free(head->lexeme);
	head->lexeme = create_expanded_lexeme(map, tokens);
	free_tokens(&tokens);
}

void	parameter_expansion(t_map *map, t_token *head)
{
	t_token	*curr;

	curr = head;
	while (curr)
	{
		if (is_token_type(curr->type, TOKEN_WORD))
			apply_expansion(map, curr);
		curr = curr->next;
	}
}
