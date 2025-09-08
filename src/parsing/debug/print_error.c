/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:35 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/08 01:18:57 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

void    print_error(t_error_type type, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    ft_dprintf(STDERR_FILENO, "bukoshell: ");
    if (type == ERROR_SYNTAX)
        ft_dprintf(STDERR_FILENO, "syntax error: ");
    else
        ft_dprintf(STDERR_FILENO, "runtime error: ");
    ft_vdprintf(STDERR_FILENO, format, args);
    va_end(args);
}
