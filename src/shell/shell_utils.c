/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:29:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/09 16:21:16 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

char	*create_identifier(t_map *map)
{
	t_environ	*user;

	if (!map)
		return (NULL);
	user = search_entry(map, "USER");
	if (!user)
	{
		user = search_entry(map, "LOGNAME");
		if (!user)
			return (NULL);
	}
	return (ft_strdup(user->value));
}

static char	*create_cwd(char *cwd_buffer)
{
	size_t	i;
	char	*cwd;
	char	**split;

	split = ft_split(cwd_buffer, '/');
	if (!split)
		return (NULL);
	if (!split[0])
	{
		free_str_arr(split, -1);
		return (ft_strdup("/ "));
	}
	i = 0;
	while (split[i + 1])
		i++;
	cwd = ft_strdup(split[i]);
	free_str_arr(split, -1);
	if (!cwd)
		return (NULL);
	return (cwd);
}

char	*set_prompt(t_shell *shell, char *identifier)
{
	char	*cwd;
	char	*prompt;
	char	**base_split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	cwd = create_cwd(shell->cwd);
	if (!cwd)
		return (NULL);
	base_split = ft_split(PS1, ' ');
	if (!base_split)
		return (NULL);
	prompt = ft_vstrjoin(8, NULL, base_split[0], " [", identifier, " ", cwd, "] ",
			base_split[1], " ");
	free_str_arr(base_split, 2);
	free(cwd);
	if (!prompt)
		return (ft_strdup(PS1));
	return (prompt);
}

t_map	*realloc_map(t_map *map, char **envp)
{
	t_map	*copy;

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
		{
			free_str_arr(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

