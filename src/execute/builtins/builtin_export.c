/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 21:03:59 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/24 11:48:49adinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <environ.h>
#include <parsing/clean.h>
#include <parsing/expand.h>
#include <execute/builtins.h>

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

static int export_variable(char *arg, t_map *map)
{
    char        **pair;

    pair = get_pair(arg);
    if (!pair)
        return (1);
    if (!is_valid_identifier(pair[0]))
    {
        log_error(ERROR_NONE, ERR_BASE, "export: '%s': not a valid identifier\n", pair[0]);
        free_str_arr(pair, -1);
        return (1);
    }
    if (!set_entry(map, pair[0], pair[1]))
    {
        free_str_arr(pair, -1);
        return (1);
    }
    free_str_arr(pair, -1);
    return (0);
}

int builtin_export(char **argv, t_map *map)
{
    size_t      i;
    t_environ   *head;
    int         status;
    char        **sorted;

    if (!argv[1])
    {
        sorted = sort_envp(map);
        if (!sorted)
            return (1);
        head = create_sorted_entries(map, sorted);
        free_str_arr(sorted, -1);
        if (!head)
            return (1);
        print_env(head, TRUE);
        return (free_entries(&head), 0);
    }
    i = 1;
    status = 0;
    while (argv[i])
    {
        if (export_variable(argv[i++], map) != 0)
            status = 1;
    }
    return (status);
}
