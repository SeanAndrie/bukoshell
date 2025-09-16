/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:35 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/16 20:14:05 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>

void	print_error(t_error_type type, const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	ft_dprintf(STDERR_FILENO, "bukoshell: ");
	if (type == ERROR_SYNTAX)
		ft_dprintf(STDERR_FILENO, "syntax error ");
	else
		ft_dprintf(STDERR_FILENO, "runtime error ");
	ft_vdprintf(STDERR_FILENO, format, args);
	va_end(args);
}

void	print_error_conditional(t_error_type type, const char *format, bool *condition, ...)
{
	va_list args;

	if (*condition)
		return ;
	*condition = true;
	va_start(args, condition);
	ft_dprintf(STDERR_FILENO, "bukoshell: ");
	if (type == ERROR_SYNTAX)
		ft_dprintf(STDERR_FILENO, "syntax error: ");
	else
		ft_dprintf(STDERR_FILENO, "runtime error: ");
	ft_vdprintf(STDERR_FILENO, format, args);
	va_end(args);
}
