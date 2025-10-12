/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:47:34 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 19:47:00 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <execute/builtins.h>
#include <parsing/clean.h>

static void	*free_helper(char **pair)
{
	if (pair)
		free_str_arr(pair, -1);
	return (NULL);
}

static void	free_entries(t_environ **entry)
{
	t_environ	*next;

	while (*entry)
	{
		next = (*entry)->next;
		free((*entry)->key);
		if ((*entry)->value)
			free((*entry)->value);
		free(*entry);
		*entry = next;
	}
}

void	free_map(t_map *map)
{
	size_t	i;

	if (!map)
		return ;
	if (map->entries)
	{
		i = 0;
		while (i < map->capacity)
		{
			free_entries(&map->entries[i]);
			i++;
		}
		free(map->entries);
	}
	free_entries(&map->order);
	free(map->order);
	free(map);
}

void update_order(t_environ **order, t_environ *entry)
{
    t_environ *curr;

    if (!order || !entry)
        return ;
    curr = *order;
    while (curr)
    {
        if (ft_strcmp(curr->key, entry->key) == 0)
        {
            if (curr->value)
                free(curr->value);
            curr->value = ft_strdup(entry->value);
            return ;
        }
        curr = curr->next;
    }
}

char	**get_pair(char *env)
{
	int		start;
	int		end;
	char	**pair;

	pair = malloc(sizeof(char *) * 3);
	if (!pair)
		return (NULL);
	start = 0;
	end = start;
	while (env[end] && env[end] != '=')
		end++;
	pair[0] = ft_substr(env, start, end);
	if (!pair[0])
		return (free_helper(pair));
	if (env[end] == '=')
	{
		pair[1] = ft_substr(env, end + 1, ft_strlen(env));
		if (!pair[1])
			return (free_helper(pair));
	}
	else
		pair[1] = NULL;
	pair[2] = NULL;
	return (pair);
}
