/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:46:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 18:09:54 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <libft.h>
#include <parsing/clean.h>

void	print_entry_info(t_environ *entry)
{
	t_environ	*curr;

	ft_printf("\nKey: %s | Value: %s\n", entry->key, entry->value);
	if (entry->next)
	{
		ft_printf("Other entries occupying this bucket:\n");
		curr = entry->next;
		while (curr)
		{
			ft_printf("Key: %s | Value: %s\n", entry->key, entry->value);
			curr = curr->next;
		}
	}
	ft_printf("\n");
}

void	print_env(t_environ *head, t_bool formatted)
{
	t_environ	*curr;

	curr = head;
	while (curr)
	{
		if (curr && !curr->readonly)
		{
			if (formatted)
			{
				ft_printf("declare -x %s", curr->key);
				if (curr->value)
					ft_printf("=\"%s\"", curr->value);
				ft_printf("\n");
			}
			else if (!formatted && curr->value)
				ft_printf("%s=%s\n", curr->key, curr->value);
		}
		curr = curr->next;
	}
}
