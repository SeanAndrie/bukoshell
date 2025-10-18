/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:12:35 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/18 23:27:16 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <bukoshell.h>

static t_bool	is_home_directory(t_map *map, char *cwd)
{
	t_environ	*home;

	if (!map || !cwd)
		return (FALSE);
	home = search_entry(map, "HOME");
	if (!home)
		return (FALSE);
	return ((ft_strcmp(home->value, cwd) == 0));
}

static char	*create_cwd(t_shell *shell)
{
	size_t	i;
	char	*cwd;
	char	**split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	if (is_home_directory(shell->map, shell->cwd))
		return (ft_strdup("~"));
	split = ft_split(shell->cwd, '/');
	if (!split)
		return (NULL);
	if (!split[0])
	{
		free_str_arr(split, -1);
		return (ft_strdup("/"));
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

char	*set_prompt(t_shell *shell, char *user)
{
	char	*cwd;
	char	*join;
	char	*prompt;
	char	**base_split;

	cwd = create_cwd(shell);
	if (!cwd)
		return (ft_strdup(PS1));
	join = ft_vstrjoin(2, " ", user, cwd);
	base_split = ft_split(PS1, ' ');
	if (!base_split)
		return (NULL);
	prompt = ft_vstrjoin(6, NULL, base_split[0], " [", join, "] ",
			base_split[1], " ");
	free_str_arr(base_split, 2);
	free(join);
	free(cwd);
	if (!prompt)
		return (ft_strdup(PS1));
	return (prompt);
}

void create_host(t_shell *shell)
{
    int     fd;
    ssize_t  bytes_read;

    fd = open("/etc/hostname", O_RDONLY);
    if (fd < 0)
        return ;
    bytes_read = read(fd, shell->host, sizeof(shell->host));
    close(fd);
    if (bytes_read < 0)
        return ;
    shell->host[bytes_read] = '\0';
    shell->host[ft_strcspn(shell->host, "\n")] = '\0';
}

char	*create_identifier(t_shell *shell)
{
    t_environ   *user;

	if (!shell->map)
		return (NULL);
	user = search_entry(shell->map, "USER");
	if (!user)
	{
		user = search_entry(shell->map, "LOGNAME");
		if (!user)
			return (NULL);
	}
    return (ft_vstrjoin(2, "@", user->value, shell->host));
}

