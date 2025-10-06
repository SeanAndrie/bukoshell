/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:33:55 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/05 22:12:08 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <expand.h>
#include <dirent.h>
#include <boolean.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static void backtrack(t_glob *g)
{
    g->p = g->star;
    g->s_back++;
    g->s = g->s_back;
}

static void advance(t_glob *g)
{
    g->p++;
    g->s++;
}

static t_bool  is_matching_pattern(char *pattern, char *str)
{
    t_glob g;

    if (*str == '.' && *pattern != '.')
        return (FALSE);
    g = (t_glob){str, pattern, NULL, NULL};
    while (*g.s)
    {
        if (*g.p == '*')
        {
            while (*g.p == '*')
                g.p++;
            g.star = g.p;
            g.s_back = g.s;
        }
        else if (*g.s == *g.p)
            advance(&g);
        else if (g.star)
           backtrack(&g);
        else
            return (FALSE);
    }
    while (*g.p == '*')
        g.p++;
    return (*g.p == '\0');
}

static t_token *create_wildcard_tokens(DIR *dir, char *pattern) 
{
    char            *res;
    char            *temp;
    t_token         *head;
    struct dirent   *entry;

    res = NULL;
    entry = NULL;
    while (TRUE)
    {
        entry = readdir(dir);
        if (!entry)
            break;
        if (!is_matching_pattern(pattern, entry->d_name))
            continue;
        temp = ft_vstrjoin(2, " ", res, entry->d_name);
        if (!temp)
        {
            if (res)
                free(res);
            free_tokens(&head);
            return (NULL);
        }
        res = temp;
    }
    return (create_tokens(res, FALSE));
}

void    apply_wildcard_expansion(t_token **head, t_token *token)
{
    DIR     *dir;
    t_token **curr;
    t_token *after;
    t_token *wc_head;

    dir = opendir(".");
    if (!dir)
        return ;
    wc_head = create_wildcard_tokens(dir, token->lexeme);
    closedir(dir);
    if (!wc_head)
        return ;
    curr = head;
    while (*curr && (*curr) != token)
        curr = &(*curr)->next;
    if (!*curr)
        return ;
    after = token->next;
    free(token->lexeme);
    free(token);
    *curr = wc_head;
    append_token(&wc_head, after);
}

