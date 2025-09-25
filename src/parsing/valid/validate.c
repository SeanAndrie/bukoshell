/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:18:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 17:40:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/valid.h>

bool	is_valid_metachar(t_token *token)
{
	if (!is_token_type(token->type, TOKEN_METACHAR))
		return (true);
	if (!token->next)
		return (print_error(ERROR_SYNTAX, "near unexpected token 'newline'\n"),
			false);
	if (is_token_type(token->type, T_PIPE))
	{
		if (!is_token_type(token->next->type, TOKEN_WORD)
			&& !is_token_type(token->next->type, TOKEN_GROUP_OPEN))
			return (print_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
					token->next->lexeme), false);
	}
	return (true);
}

bool	validate_tokens(t_token *head)
{
	t_token	*curr;

	curr = head;
	if (curr && is_token_type(curr->type, TOKEN_CTRL_OP))
		return (print_error(ERROR_SYNTAX, "unexpected operator '%s'\n",
				curr->lexeme), false);
	if (!parse_command_list(&curr))
		return (false);
	return (true);
}
