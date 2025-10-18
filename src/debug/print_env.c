/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 02:46:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/18 08:15:47 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <environ.h>
#include <parsing/clean.h>
#include <parsing/expand.h>

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

static void *free_helper(t_environ *head)
{
    if (head)
        free_entries(&head);
    return (NULL);
}

static char **sort_envp(t_map *map)
{
    size_t  size;
    char    **sorted;

    sorted = map_to_envp(map);
    if (!sorted)
        return (NULL);
    size = environ_size(sorted);
    quick_sort(sorted, 0, size - 1);
    return (sorted);
}

static t_environ *create_sorted_entries(t_map *map, char **sorted)
{
    size_t      i;
    t_environ   *head;
    t_environ   *copy;
    t_environ   *entry;
    char        **pair;

    head = NULL;
    i = -1;
    while (sorted[++i])
    {
        pair = get_pair(sorted[i]);
        if (pair)
        {
            entry = search_entry(map, pair[0]);
            if (!entry)
                return (free_helper(head));
            copy = create_entry(entry->key, entry->value);
            if (!copy)
                return (free_helper(head));
            copy->readonly = entry->readonly;
            append_entry(&head, copy);
        }
        free_str_arr(pair, -1);
    }
    return (head);
}

void    print_env(t_map *map, t_bool formatted)
{
    t_environ   *head;
    t_environ   *curr;
    char        *format;
    char        **sorted;
    
    sorted = sort_envp(map);
    if (!sorted)
        return ;
    head = create_sorted_entries(map, sorted);
    free_str_arr(sorted, -1);
    if (!head)
        return ;
    if (formatted)
        format = "declare -x %s=\"%s\"\n";
    else
        format = "%s=%s\n";
    curr = head;
    while (curr)
    {
        if (!curr->readonly)
            ft_printf(format, curr->key, curr->value);
        curr = curr->next;
    }
    free_entries(&head);
}
