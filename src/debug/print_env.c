/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:46:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/17 10:21:27 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>

void    print_entry_info(t_environ *entry)
{
    t_environ   *curr;

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

void    print_env(t_environ *head)
{
    ft_printf("\n");
    while (head)
    {
        ft_printf("%s=%s\n", head->key, head->value);
        head = head->next;
    }
    ft_printf("\n");
}
