/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:18:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/28 16:31:41 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/clean.h>
#include <parsing/valid.h>

void	consume(t_token **curr)
{
	if (*curr)
		*curr = (*curr)->next;
}

t_bool	validate_tokens(t_token *head)
{
	t_token	*curr;

	curr = head;
	if (curr && is_token_type(curr->type, TOKEN_CTRL_OP))
		return (log_error(ERROR_SYNTAX, "unexpected operator '%s'\n",
				curr->lexeme), FALSE);
	if (!parse_command_list(&curr, FALSE))
		return (FALSE);
	return (TRUE);
}
