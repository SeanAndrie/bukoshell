/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:21:58 by ccastro           #+#    #+#             */
/*   Updated: 2025/10/02 03:10:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <signals.h>
#include <readline/readline.h>

volatile sig_atomic_t g_signal = 128;

static int	heredoc_event_hook(void)
{
	if (g_signal > 128)
		g_signal = 128;
	return (0);
}

static void handle_sigint_heredoc(int sig)
{
    (void)sig;
    write(STDERR_FILENO, "\n", 1);
    g_signal += 2;
    rl_done = 1;
}

static void	handle_sigint_prompt(int sig)
{
    (void)sig;
    write(STDERR_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void set_signals_heredoc(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    rl_event_hook = heredoc_event_hook;
    sa_int.sa_handler = handle_sigint_heredoc;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void set_signals_prompt(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = handle_sigint_prompt;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

