/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:46:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 12:35:20 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>

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
    ft_printf("\n");
    while (head)
    {
        if (!head->readonly)
        {
            if (formatted)
                ft_printf("declare -x %s", head->key);
            else
                ft_printf("%s", head->key);
            if (head->value)
            {
                if (formatted)
                    ft_printf("=\"%s\"", head->value);
                else
                    ft_printf("=%s", head->value);
            }
            ft_printf("\n");
        }
        head = head->next;
    }
    ft_printf("\n");
}
