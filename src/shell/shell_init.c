/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/18 23:22:17 by sgadinga         ###   ########.fr       */
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
    shell->envp = copy_envp(envp);
    ft_memset(shell->cwd, 0, sizeof(shell->cwd));
    ft_memset(shell->host, 0, sizeof(shell->host));
    shell->map = create_map(environ_size(shell->envp));
    return (shell);
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

static void update_status(t_shell *shell)
{
    char *status;

    if (!shell || !shell->map)
        return ;
    shell->status = g_signal;
    status = ft_itoa(shell->status);
    if (!status)
        status = ft_strdup("0");
    set_entry(shell->map, "?", status);
    free(status);
    g_signal = 0;
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
    if (DEBUG_MODE)
        print_syntax_tree(shell->root);
    return (TRUE);
}

void	start_shell(t_shell *shell)
{
    resolve_map_changes(shell);
    if (!start_parser(shell))
        g_signal = 127;
    else if (g_signal == 0)
        g_signal = exec_node(shell->root, shell->map, shell->envp);
    free_shell(shell, FALSE);
    update_status(shell);
}
