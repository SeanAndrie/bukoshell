/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:53:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/24 17:41:05 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <environ.h>
#include <expand.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static t_bool	handle_expansion(t_map *map, t_token *token)
{
	char		*key;
	char		*value;
	t_environ	*entry;

	key = ft_substr(token->lexeme, 1, ft_strlen(token->lexeme) - 1);
	if (!key)
		return (FALSE);
	entry = search_entry(map, key);
	free(key);
	free(token->lexeme);
	if (!entry)
	{
		token->lexeme = NULL;
		return (FALSE);
	}
	value = ft_strdup(entry->value);
	if (!value)
		return (FALSE);
	token->lexeme = value;
	return (TRUE);
}

static char	*create_expanded_lexeme(t_map *map, t_token **tokens)
{
	t_token	*curr;
	t_token	*concat;
	char	*lexeme;

	curr = *tokens;
	while (curr)
	{
		if (!handle_expansion(map, curr))
			curr->type = TOKEN_NONE;
		curr = curr->next;
	}
	remove_tokens(tokens, TOKEN_NONE);
	concat = concat_tokens(*tokens, TOKEN_WORD);
	free_tokens(tokens);
	if (!concat)
		return (NULL);
	lexeme = ft_strdup(concat->lexeme);
	free_tokens(&concat);
	if (!lexeme)
		return (NULL);
	return (lexeme);
}

void	apply_param_expansion(t_token *token, t_map *map, t_bool heredoc)
{
	t_token	*tokens;

	if (!token->expandable)
		return ;
	if (is_token_type(token->type, T_WORD_SQUOTE) || ft_strcmp(token->lexeme,
			"$") == 0)
		return ;
	if (!*token->lexeme)
		return ;
	tokens = create_tokens(token->lexeme, TRUE, heredoc);
	if (!tokens)
		return ;
	free(token->lexeme);
	token->lexeme = create_expanded_lexeme(map, &tokens);
}
