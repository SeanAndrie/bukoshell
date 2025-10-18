/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:18:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/18 09:00:20 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <boolean.h>
#include <parsing/parsing.h>

void	consume(t_token **curr)
{
	if (*curr)
		*curr = (*curr)->next;
}

t_bool	check_arithmetic(t_token *head)
{
	unsigned int	mask;

	if (!head)
		return (FALSE);
	mask = create_token_mask(head);
	if ((mask & TOKEN_ARITH) && !(mask & TOKEN_SUBSHELL))
	{
		log_error(ERROR_SYNTAX, ERR_BASE,
			"arithmetic expressions are not supported\n");
		return (TRUE);
	}
	return (FALSE);
}

t_bool 	is_separated_command(t_token *token)
{
	if (token && !is_token_type(token->type, TOKEN_CTRL_OP)
		&& !is_token_type(token->type, TOKEN_GROUP_CLOSE))
	{
		log_error(ERROR_SYNTAX, ERR_BASE, "near unexpected token '%s'\n",
			token->lexeme);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	validate_tokens(t_token *head) 
{
	t_token	*curr;
	int		depth;

	depth = 0;
	curr = head;
	if (curr && is_token_type(curr->type, TOKEN_CTRL_OP))
    {
        log_error(ERROR_SYNTAX, ERR_BASE, "unexpected operator '%s'\n", curr->lexeme);
        return (FALSE);
    }
	if (!parse_command_list(&curr, &depth))
		return (FALSE);
	if (curr && is_token_type(curr->type, TOKEN_GROUP_CLOSE))
	{
	    log_error(ERROR_SYNTAX, ERR_BASE, "near unexpected token '%s'\n", curr->lexeme);
	    return (FALSE);
	}
	return (TRUE);
}
