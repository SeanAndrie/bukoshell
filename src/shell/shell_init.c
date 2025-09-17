/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/17 12:50:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

char	*create_identifier(t_map *map)
{
	t_environ	*hostname;
	t_environ	*username;
	char		*identifier;

	if (!map)
		return (NULL);
	username = search_entry(map, "USER");
	if (!username)
		return (NULL);
	hostname = search_entry(map, "HOSTNAME");
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
	temp = ft_vstrjoin(4, " ", base_split[0], identifier, cwd_split[i - 1],
			base_split[1]);
	prompt = ft_strjoin(temp, " ");
	free(temp);
	free_str_arr(cwd_split, i);
	free_str_arr(base_split, 2);
	return (prompt); // if NULL, it'll return NULL anways :P
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	handle_signals();
	shell->status = 0;
	shell->line = NULL;
	shell->head = NULL;
	shell->root = NULL;
	ft_memset(shell->cwd, 0, sizeof(shell->cwd));
	shell->map = init_environ(envp);
	return (shell);
}

static bool	parse_prompt(t_shell *shell)
{
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (false);
	if (!handle_concatenation(&shell->head))
		return (false);
	remove_tokens(&shell->head, TOKEN_WHITESPACE);
	if (DEBUG_MODE)
		print_tokens(shell->head, true);
	if (!validate_tokens(shell->head))
		return (false);
	shell->root = create_syntax_tree(shell->head, NULL);
	if (!shell->root)
		return (false);
	if (DEBUG_MODE)
		print_syntax_tree(shell->root);
	return (true);
}

int	start_shell(t_shell *shell)
{
	if (!parse_prompt(shell))
	{
		free_shell(shell, false);
		return (2);
	}
	free_shell(shell, false);
	return (shell->status);
}
