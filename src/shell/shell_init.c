/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/23 23:47:48by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

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
        shell->map = realloc_map(shell->map);
}

static void update_variables(t_shell *shell)
{
    size_t  i;
    char    *status;

    if (!shell || !shell->map)
        return ;
    shell->status = g_signal;
    status = ft_itoa(shell->status);
    if (!status)
        status = ft_strdup("0");
    set_entry(shell->map, "?", status);
    free(status);
    if (shell->root && shell->root->argv && *shell->root->argv)
    {
        i = 0;
        while (shell->root->argv[i])
            i++;
        set_entry(shell->map, "_", shell->root->argv[i - 1]);
    }
    g_signal = 0;
}

static int start_parser(t_shell *shell)
{
    shell->head = create_tokens(shell->line, FALSE, FALSE);
    if (!shell->head)
        return (2);
    if (!normalize_tokens(&shell->head, shell->map))
        return (2);
    if (DEBUG_MODE)
        print_tokens(shell->head, TRUE);
    if (!validate_tokens(shell->head))
        return (2);
    shell->root = create_syntax_tree(shell->head, NULL);
    if (!shell->root)
        return (2);
    collect_heredocs(shell->root, shell->map);
    if (g_signal != 0)
        return (g_signal);
    if (DEBUG_MODE)
        print_syntax_tree(shell->root);
    return (0);
}

static int start_execution(t_shell *shell)
{
    int status;
    
    if (!shell->root)
        return (1);
    status = exec_node(shell->root, shell->map, shell->envp);
    return (status);
}

void	start_shell(t_shell *shell)
{
    resolve_map_changes(shell);
    if (g_signal == 0)
        g_signal = start_parser(shell);
    if (g_signal == 0)
        g_signal = start_execution(shell); 
    update_variables(shell);
    free_shell(shell, FALSE);
}
