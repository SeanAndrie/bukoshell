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

#include <libft.h>
#include <parsing/parsing.h>

void	consume(t_token **curr)
{
	if (*curr)
		*curr = (*curr)->next;
}

t_bool  is_arithmetic(t_token *head)
{
    t_token *copy;
    t_token *curr;
    int     depth;

    copy = copy_tokens(head, NULL);
    if (!copy)
        return (FALSE);
    if(!handle_concatenation(&copy, TOKEN_ARITH, TOKEN_ARITH))
        return(free_tokens(&copy), FALSE);
    depth = 0;
    curr = copy;
    while (curr)
    {
        if (curr->lexeme[0] == '(')
            curr->type |= TOKEN_GROUP_OPEN;
        else if (curr->lexeme[0] == ')')
            curr->type |= TOKEN_GROUP_CLOSE;
        track_depth(curr, &depth, TOKEN_ARITH);
        curr = curr->next;
    }
    free_tokens(&copy);
    if (depth != 0)
        return (FALSE);
    return (TRUE);
}

t_bool	validate_tokens(t_token *head)
{
	t_token	*curr;

	curr = head;
	if (curr && is_token_type(curr->type, TOKEN_CTRL_OP))
		return (log_error(ERROR_SYNTAX, ERR_BASE, "unexpected operator '%s'\n",
				curr->lexeme), FALSE);
	if (!parse_command_list(&curr, FALSE))
		return (FALSE);
	return (TRUE);
}
