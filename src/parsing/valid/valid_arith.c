/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_arith.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 00:02:19 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 03:53:57 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/clean.h>
#include <parsing/valid.h>

t_bool	is_valid_arithmetic(t_token *start, t_token *end)
{
	t_token			*trim;
	unsigned int	mask;

	trim = copy_tokens(start, end);
	if (!trim)
		return (FALSE);
	remove_tokens(&trim, TOKEN_ARITH);
	print_tokens(trim, FALSE);
	mask = create_token_mask(trim);
	if (!(mask & TOKEN_CTRL_OP))
		return (free_tokens(&trim), FALSE);
	if (!validate_tokens(trim))
		return (free_tokens(&trim), FALSE);
	free_tokens(&trim);
	return (TRUE);
}

t_bool	validate_arithmetic(t_token *start, t_token *end)
{
	t_token	*op;

	if (!start)
		return (FALSE);
	if (start == end)
		return (is_valid_arithmetic(start, end));
	op = find_lowest_precedence(start, end);
	if (!op)
		return (is_valid_arithmetic(start, end));
	if (!validate_arithmetic(start, op))
		return (FALSE);
	if (!validate_arithmetic(op->next, end))
		return (FALSE);
	return (TRUE);
}
