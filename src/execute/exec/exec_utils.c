/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 22:36:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 16:30:32 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <execute/execute.h>

char    *find_cmd_path(char *cmd, t_environ *path_var)
{
    size_t      i;
    char        **paths;
    char        *cmd_path;

    if (!cmd || !path_var || !path_var->value)
        return (NULL);
    paths = ft_split(path_var->value, ':');
    if (!paths)
        return (NULL);
    i = -1;
    while (paths[++i])
    {
        cmd_path = ft_vstrjoin(2, "/", paths[i], cmd);
        if (!cmd_path)
        {
            free_str_arr(paths, i);
            return (NULL);
        }
        if (access(cmd_path, X_OK) == 0)
            break;
    }
    free_str_arr(paths, i);
    return (cmd_path);
}

void restore_fds(int in, int out)
{
    if (dup2(in, STDIN_FILENO) < 0)
        perror("dup2");
    close(in);
    if (dup2(out, STDOUT_FILENO) < 0)
        perror("dup2");
    close(out);
}

static t_bool open_file(t_redirect *redir)
{
    if (is_token_type(redir->type, T_REDIR_OUT))
        redir->fd = open(redir->fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (is_token_type(redir->type, T_REDIR_APPEND))
        redir->fd = open(redir->fname, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (is_token_type(redir->type, T_REDIR_IN))
        redir->fd = open(redir->fname, O_RDONLY);
    else
        return (FALSE);
    if (redir->fd < 0)
    {
        perror(redir->fname);
        return (FALSE);
    }
    return (TRUE);
}

static void redirect_fds(t_redirect *redir)
{
    if (is_token_type(redir->type, T_REDIR_IN))
    {
        if (dup2(redir->fd, STDIN_FILENO) < 0)
            perror("dup2");
    }
    else
    {
        if (dup2(redir->fd, STDOUT_FILENO) < 0)
            perror("dup2");
    }
}

t_bool handle_redirections(t_redirect *head)
{
    t_redirect *curr;

    if (!head)
        return (FALSE);
    curr = head;
    while (curr)
    {
        if (!open_file(curr))
        {
            free_redirects(&head, TRUE);
            return (FALSE);
        }
        curr = curr->next;
    }
    curr = head;
    while (curr)
    {
        redirect_fds(curr);
        close(curr->fd);
        curr = curr->next;
    }
    return (TRUE);
}

