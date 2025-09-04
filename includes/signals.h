/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:19:37 by ccastro           #+#    #+#             */
/*   Updated: 2025/09/04 08:19:03 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <libft.h>
# include <bukoshell.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

# ifndef ECHOCTL
#  define ECHOCTL 0x00000040
# endif

void	handle_signals(void);
void	disable_echoctl(struct termios *term);

#endif