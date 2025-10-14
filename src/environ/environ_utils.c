/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:47:20 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 20:39:39 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>

static t_bool	clear_entry(t_environ **head, char *key)
{
	t_environ	*temp;
	t_environ	**curr;

	if (!head || !*head || !key)
		return (FALSE);
	curr = head;
	while (*curr)
	{
		if (ft_strcmp((*curr)->key, key) == 0)
		{
			temp = *curr;
			*curr = (*curr)->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return (TRUE);
		}
		curr = &(*curr)->next;
	}
	return (FALSE);
}

t_environ	*search_entry(t_map *map, char *key)
{
	size_t		index;
	t_environ	*entry;

	if (!map || !key)
		return (NULL);
	index = hash_djb2(key) % map->capacity;
	entry = map->entries[index];
	if (!entry)
		return (NULL);
	while (entry)
	{
		if (ft_strcmp(entry->key, key) == 0)
			return (entry);
		entry = entry->next;
	}
	return (NULL);
}

t_bool	insert_entry(t_map *map, char *key, char *value, t_bool readonly)
{
	size_t		index;
	t_environ	*entry;
	t_environ	*order_entry;

	if (!map || !map->entries || !key)
		return (FALSE);
	entry = create_entry(key, value);
	if (!entry)
		return (FALSE);
    entry->readonly = readonly;
	order_entry = create_entry(entry->key, entry->value);
	if (!order_entry)
	{
		free(entry);
		return (FALSE);
	}
    order_entry->readonly = readonly;
	index = hash_djb2(key) % map->capacity;
	append_entry(&map->entries[index], entry);
	append_entry(&map->order, order_entry);
	map->size++;
	map->load_factor = (double)map->size / (double)map->capacity;
	return (TRUE);
}

t_bool	set_entry(t_map *map, char *key, char *value, t_bool readonly)
{
	t_environ	*target;

	if (!map)
		return (FALSE);
	target = search_entry(map, key);
	if (!target)
	{
		if (!insert_entry(map, key, value, readonly))
			return (FALSE);
		map->modified = TRUE;
		return (TRUE);
	}
	if (target->value)
		free(target->value);
	target->value = ft_strdup(value);
	if (!target->value)
		return (FALSE);
    target->readonly = readonly;
	update_order(&map->order, target, readonly);
	map->modified = TRUE;
	return (TRUE);
}

t_bool	delete_entry(t_map *map, char *key)
{
	size_t		index;
	t_environ	*entry;

	if (!map || !map->entries || !key)
		return (FALSE);
	index = hash_djb2(key) % map->capacity;
	entry = map->entries[index];
	if (!entry)
		return (FALSE);
	if (!clear_entry(&map->entries[index], key))
		return (FALSE);
	clear_entry(&map->order, key);
	map->size--;
	map->load_factor = (double)map->size / (double)map->capacity;
	map->modified = TRUE;
	return (TRUE);
}
