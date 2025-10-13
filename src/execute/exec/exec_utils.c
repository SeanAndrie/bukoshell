/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 22:36:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 13:12:52 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <execute/execute.h>

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

