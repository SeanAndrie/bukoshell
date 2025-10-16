/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:53:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:59:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <expand.h>
#include <environ.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static t_bool	free_helper(char *s)
{
	if (s)
		free(s);
	return (FALSE);
}

static t_bool	handle_expansion(t_map *map, t_token *token)
{
	char		*key;
	char		*temp;
	t_environ	*entry;

	temp = ft_strdup(token->lexeme);
	if (!temp)
		return (FALSE);
	key = ft_substr(token->lexeme, 1, ft_strlen(token->lexeme) - 1);
	if (!key)
		return (free_helper(temp));
	entry = search_entry(map, key);
	free(key);
	if (!entry)
		return (free_helper(temp));
	free(token->lexeme);
	token->lexeme = ft_strdup(entry->value);
	if (!token->lexeme)
	{
		token->lexeme = temp;
		return (FALSE);
	}
	free(temp);
	return (TRUE);
}

static char	*create_expanded_lexeme(t_map *map, t_token *tokens)
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

void	apply_param_expansion(t_token *token, t_map *map, t_bool heredoc)
{
	t_token	*tokens;

	if (is_token_type(token->type, T_WORD_SQUOTE))
		return ;
	if (!*token->lexeme)
		return ;
	tokens = create_tokens(token->lexeme, TRUE, heredoc);
	if (!tokens)
		return ;
	free(token->lexeme);
	token->lexeme = create_expanded_lexeme(map, tokens);
	free_tokens(&tokens);
}
