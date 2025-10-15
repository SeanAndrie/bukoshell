/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/15 13:28:32 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->status = 0;
	shell->line = NULL;
	shell->head = NULL;
	shell->root = NULL;
	shell->token_mask = 0;
	shell->envp = copy_envp(envp);
	ft_memset(shell->cwd, 0, sizeof(shell->cwd));
	shell->map = create_map(environ_size(shell->envp));
	return (shell);
}

void	init_shell_variables(t_map *map)
{
	t_environ	*shlvl;
	char		*level;
	t_environ	*status;

	set_entry(map, "?", "0");
	status = search_entry(map, "?");
	if (status)
		status->readonly = TRUE;
	set_entry(map, "OLDPWD", "");
	shlvl = search_entry(map, "SHLVL");
	if (!shlvl)
	{
		set_entry(map, "SHLVL", "0");
		shlvl = search_entry(map, "SHLVL");
	}
	level = ft_itoa(ft_atoi(shlvl->value) + 1);
	set_entry(map, "SHLVL", level);
	free(level);
}

static void	resolve_map_changes(t_shell *shell)
{
	char	**temp;

	if (shell->map->modified)
	{
		temp = shell->envp;
		free_str_arr(shell->envp, -1);
		shell->envp = map_to_envp(shell->map);
		if (!shell->envp)
			shell->envp = temp;
		shell->map->modified = FALSE;
	}
	if (shell->map->load_factor >= LOAD_THRESHOLD)
		shell->map = realloc_map(shell->map, shell->envp);
}

static t_bool	start_parser(t_shell *shell)
{
	shell->head = create_tokens(shell->line, FALSE, FALSE);
	if (!shell->head)
		return (FALSE);
	if (!normalize_tokens(&shell->head, shell->map))
		return (FALSE);
	if (DEBUG_MODE)
		print_tokens(shell->head, TRUE);
	shell->root = create_syntax_tree(shell->head, NULL);
	if (!shell->root)
		return (FALSE);
	collect_heredocs(shell->root, shell->map);
	if (!validate_tokens(shell->head))
		return (FALSE);
	shell->token_mask = create_token_mask(shell->head);
	if (!check_arithmetic(shell->head, shell->token_mask))
		return (FALSE);
	if (DEBUG_MODE)
		print_syntax_tree(shell->root);
	return (TRUE);
}

void	start_shell(t_shell *shell)
{
	resolve_map_changes(shell);
	if (!start_parser(shell))
	{
		g_signal = 127;
		free_shell(shell, FALSE);
		return ;
	}
	if (g_signal == 0)
		g_signal = exec_node(shell->root, shell->map, shell->envp);
	shell->token_mask = 0;
	free_shell(shell, FALSE);
}
