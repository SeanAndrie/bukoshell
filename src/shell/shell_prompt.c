/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:21:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/15 13:29:44 by sgadinga         ###   ########.fr       */
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
	char	*join;
	char	*prompt;
	char	**base_split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	cwd = create_cwd(shell->cwd);
	if (!cwd)
		return (NULL);
	join = ft_vstrjoin(2, " ", identifier, cwd);
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

t_bool	check_arithmetic(t_token *head, unsigned int mask)
{
	if ((mask & TOKEN_ARITH) && is_arithmetic(head))
	{
		log_error(ERROR_SYNTAX, ERR_BASE,
			"arithmetic expressions are not supported\n");
		return (FALSE);
	}
	return (TRUE);
}

