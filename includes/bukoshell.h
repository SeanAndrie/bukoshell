/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 08:16:13 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <parsing.h>
# include <signals.h>

# define EXIT "exit\n"

extern int exit_status;

// Debugging
void	print_tokens(t_token *head, bool show_whitespace);

#endif