/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:18:16 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:21:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/valid.h>

bool	are_valid_tokens(t_token *head)
{
	t_token	*prev;
	t_token	*curr;
	int		depth;

	prev = NULL;
	curr = head;
	depth = 0;
	while (curr)
	{
		if (!is_valid_grouping(prev, curr, &depth) || !is_valid_operator(curr)
			|| !is_valid_redirect(curr) || !is_valid_parameter(curr))
			return (false);
		curr = curr->next;
	}
	if (depth != 0)
	{
		print_error(ERROR_SYNTAX, "unmatched '('\n");
		return (false);
	}
	return (true);
}
