/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:33:55 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/19 19:10:58 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <dirent.h>
#include <expand.h>
#include <boolean.h>
#include <parsing/lexer.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static size_t count_matching_paths(char *pattern)
{
    DIR             *dir;
    size_t          size;
    struct dirent   *entry;

    dir = opendir(".");
    if (!dir)
        return (0);
    size = 0;
    while (TRUE)
    {
        entry = readdir(dir);
        if (!entry)
            break;
        if (!is_matching_pattern(pattern, entry->d_name))
            continue;
        size++;
    }
    closedir(dir);
    return (size);
}

static char **create_paths(DIR *dir, char *pattern, size_t size)
{
    size_t          i;
    struct dirent   *entry;
    char            **paths;

    paths = ft_calloc(size + 1, sizeof(char *));
    if (!paths)
        return (NULL);
    i = 0;
    while (TRUE)
    {
        entry = readdir(dir);
        if (!entry)
            break;
        if (!is_matching_pattern(pattern, entry->d_name))
            continue;
        paths[i] = ft_strdup(entry->d_name);
        if (!paths[i])
        {
            free_str_arr(paths, i);
            return (NULL);
        }
        i++;
    }
    return (paths);
}

static t_token *create_path_tokens(char **paths)
{
    size_t  i;
    char    *temp;
    char    *concat;
    t_token *tokens;

    tokens = NULL;
    concat = NULL;
    i = -1;
    while (paths[++i])
    {
        temp = ft_vstrjoin(2, " ", concat, paths[i]);
        if (!temp)
        {
            free(concat);
            return (NULL);
        }
        free(concat);
        concat = temp;
    }
    tokens = create_tokens(concat, FALSE, TRUE);
    free(concat);
    handle_concatenation(&tokens, TOKEN_WORD);
    if (!tokens)
        return (NULL);
    return (tokens);
}

static void attach_path_head(t_token **head, t_token *path_tokens, t_token *wc_token)
{
    t_token **curr;
    t_token *after;

    curr = head;
    while (*curr && *curr != wc_token)
        curr = &(*curr)->next;
    after = wc_token->next;
    free((*curr)->lexeme);
    free(*curr);
    *curr = path_tokens;
    append_token(&path_tokens, after);
}

t_bool apply_wildcard_expansion(t_token **head, t_token *wc_token)
{
    DIR     *dir;
    size_t  count;
    char    **paths;
    t_token *tokens;

    if (!head || !*head || !wc_token->expandable)
        return (FALSE);
    dir = opendir(".");
    if (!dir)
        return (FALSE);
    count = count_matching_paths(wc_token->lexeme);
    if (!count)
        return (FALSE);
    paths = create_paths(dir, wc_token->lexeme, count);
    closedir(dir);
    if (!paths)
        return (FALSE);
    quick_sort(paths, 0, count - 1);
    tokens = create_path_tokens(paths);
    free_str_arr(paths, count);
    if (!tokens)
        return (FALSE);
    attach_path_head(head, tokens, wc_token);
    return (TRUE);
}
