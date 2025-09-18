/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:29:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/18 17:51:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

char	*create_identifier(t_map *map)
{
	t_environ	*username;
	t_environ	*hostname;
	char		*identifier;

	if (!map)
		return (NULL);
	username = search_entry(map, "USER");
	if (!username)
	{
		username = search_entry(map, "LOGNAME");
		if (!username)
			return (NULL);
	}
	hostname = search_entry(map, "NAME");
	if (!hostname)
		return (ft_strdup(username->value));
	identifier = ft_vstrjoin(2, "@", username->value, hostname->value);
	if (!identifier)
		return (NULL);
	return (identifier);
}

char	*set_cwd_prompt(t_shell *shell, char *identifier)
{
	size_t	i;
	char	*temp;
	char	*prompt;
	char	**cwd_split;
	char	**base_split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	cwd_split = ft_split(shell->cwd, '/');
	if (!cwd_split)
		return (NULL);
	i = 0;
	while (cwd_split[i])
		i++;
	base_split = ft_split(PS1, ' ');
	if (!base_split)
		return (free_str_arr(cwd_split, -1), NULL);
	temp = ft_vstrjoin(6, " ", base_split[0], "[", identifier, cwd_split[i - 1],
			"]", base_split[1]);
	prompt = ft_strjoin(temp, " ");
	free(temp);
	free_str_arr(cwd_split, i);
	free_str_arr(base_split, 2);
	return (prompt);
}

char	**copy_envp(char **envp)
{
	size_t	i;
	char	**copy;

	if (!envp)
		return (NULL);
	copy = malloc(sizeof(char *) * (environ_size(envp) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_str_arr(copy, i), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_map	*realloc_map(t_map *map, char **envp)
{
	t_map *copy;

	if (!envp)
		return (NULL);
	copy = create_map(map->capacity);
	if (!copy)
		return (NULL);
	init_environ(copy, envp);
	free_map(map);
	if (!copy)
		return (NULL);
	return (copy);
}