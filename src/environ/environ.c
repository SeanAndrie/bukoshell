/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:47:34 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/19 20:00:47 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fcntl.h>
#include <environ.h>
#include <parsing/clean.h>

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

char	**get_pair(char *env)
{
	char	**pair;
	char	*equal;

	pair = malloc(sizeof(char *) * 3);
	if (!pair)
		return (NULL);
	equal = ft_strchr(env, '=');
	if (equal)
	{
		pair[0] = ft_substr(env, 0, equal - env);
		pair[1] = ft_strdup(equal + 1);
	}
	else
	{
		pair[0] = ft_strdup(env);
		pair[1] = ft_strdup("");
	}
	if (!pair[0] && !pair[1])
	{
		free_str_arr(pair, 2);
		return (NULL);
	}
	pair[2] = NULL;
	return (pair);
}

static void special_variables(t_map *map)
{
    int         fd;
	t_environ	*status;
    t_environ   *hostname;
    ssize_t     bytes_read;
    char        buffer[64];

	set_entry(map, "?", "0");
	status = search_entry(map, "?");
	if (status)
		status->readonly = TRUE;
	set_order(&map->order, status);
    fd = open("/etc/hostname", O_RDONLY);
    if (fd < 0)
        return ;
    bytes_read = read(fd, buffer, sizeof(buffer));
    close(fd);
    if (bytes_read < 0)
        return ;
    buffer[bytes_read] = '\0';
    buffer[ft_strcspn(buffer, "\n")] = '\0';
    set_entry(map, "HOSTNAME", buffer);
    hostname = search_entry(map, "HOSTNAME");
    set_order(&map->order, hostname);
}

static void	init_variables(t_map *map)
{
	t_environ	*pid;
	t_environ	*shlvl;
	char		*value;

	set_entry(map, "OLDPWD", "");
	shlvl = search_entry(map, "SHLVL");
	if (!shlvl)
	{
		set_entry(map, "SHLVL", "0");
		shlvl = search_entry(map, "SHLVL");
	}
	value = ft_itoa(ft_atoi(shlvl->value) + 1);
	if (!value)
		return ;
	set_entry(map, "SHLVL", value);
	free(value);
	value = ft_itoa(getpid());
	if (!value)
		return ;
	set_entry(map, "$", value);
	free(value);
	pid = search_entry(map, "$");
	if (pid)
		pid->readonly = TRUE;	
	set_order(&map->order, pid);
}

void	init_environ(t_map *map, char **envp)
{
	char	**pair;

	while (*envp)
	{
		if (**envp)
		{
			pair = get_pair(*envp);
			if (pair)
			{
				if (!insert_entry(map, pair[0], pair[1]))
				{
					free_str_arr(pair, -1);
					return ;
				}
				free_str_arr(pair, -1);
			}
		}
		envp++;
	}
    special_variables(map);
    init_variables(map);
}
