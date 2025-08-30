/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:29:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/30 20:30:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

void	free_tokens(t_token **head)
{
	t_token	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->lexeme);
		free(*head);
		*head = next;
	}
}
