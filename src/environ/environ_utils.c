/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:47:20 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 01:33:03 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>

static void	clear_entry(t_environ **head, char *key)
{
	t_environ	*temp;
	t_environ	**curr;
	size_t		key_len;

	curr = head;
	key_len = ft_strlen(key);
	while (*curr)
	{
		if (ft_strncmp((*curr)->key, key, key_len) == 0)
		{
			temp = *curr;
			*curr = (*curr)->next;
			temp->next = NULL;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		else
			curr = &(*curr)->next;
	}
}

t_environ	*search_entry(t_map *map, char *key)
{
	size_t		index;
	t_environ	*entry;
	size_t		key_len;

	if (!map || !key)
		return (NULL);
	key_len = ft_strlen(key);
	index = hash_djb2(key) % map->capacity;
	entry = map->entries[index];
	if (!entry)
		return (NULL);
	while (entry)
	{
		if (ft_strncmp(entry->key, key, key_len) == 0)
			return (entry);
		entry = entry->next;
	}
	return (NULL);
}

t_bool	insert_entry(t_map *map, char *key, char *value)
{
	size_t		index;
	t_environ	*entry;
	t_environ	*order_entry;

	if (!map || !map->entries || !key)
		return (FALSE);
	entry = create_entry(key, value);
	if (!entry)
		return (FALSE);
	order_entry = create_entry(entry->key, entry->value);
	if (!order_entry)
		return (free(entry), FALSE);
	index = hash_djb2(key) % map->capacity;
	append_entry(&map->entries[index], entry);
	append_entry(&map->order, order_entry);
	map->size++;
	map->load_factor = (double)map->size / (double)map->capacity;
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
	clear_entry(&map->entries[index], key);
	clear_entry(&map->order, key);
	map->size--;
	map->load_factor = (double)map->size / (double)map->capacity;
	return (TRUE);
}
