/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 01:49:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/18 17:54:49 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <expand.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

// bool	normal_expansion(t_map *map, t_token *curr)
// {
// 	char			*strip;
// 	t_token			*tokens;
// 	unsigned int	token_mask;

// 	token_mask = 0;
// 	(void)map;
// 	strip = ft_strtrim(curr->lexeme, "\"");
// 	if (!strip)
// 		return (false);
// 	tokens = create_tokens(strip);
// 	free(strip);
// 	if (!tokens)
// 		return (false);
// 	token_mask = create_token_mask(tokens);
// 	if (token_mask & TOKEN_PARAMETER)
// 	{
// 		print_tokens(tokens, false);
// 	}
// 	free_tokens(&tokens);
// 	return (true);
// }

void	parameter_expansion(t_map *map, t_token *head)
{
	t_token	*curr;

	if (!head)
		return ;
	curr = head;
	while (curr)
	{
		if (is_token_type(curr->type, TOKEN_WORD))
		{
			if (!normal_expansion(map, curr))
				return ;
		}
		curr = curr->next;
	}
}
