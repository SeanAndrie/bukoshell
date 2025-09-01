/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:21:58 by ccastro           #+#    #+#             */
/*   Updated: 2025/09/01 17:42:45 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>

static void	handle_sigint(int sig);

static void	handle_sigint(int sig)
{
	(void) sig;
	rl_replace_line("", 0);
	ft_printf("\n");
	rl_redisplay();
}

void	disable_echoctl(struct termios *term)
{
	tcgetattr(STDIN_FILENO, term);
	term->c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, term);
}

void	handle_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
