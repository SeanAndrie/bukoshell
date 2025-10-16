/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dir_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:48:49 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:22:23 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <dirent.h>
#include <boolean.h>

void print_dir_list(void)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (!dir)
    {
        log_error(ERROR_WARNING, ERR_BASE, "opendir error\n");
        return ;
    }
    entry = NULL;
    while (TRUE)
    {
        entry = readdir(dir);
        if (!entry)
            break;
        ft_printf("%s\n", entry->d_name);
    }
    if (closedir(dir) == -1)
    {
        log_error(ERROR_WARNING, ERR_BASE, "closedir error\n");
        return ;
    }
    ft_printf("\n");
}

