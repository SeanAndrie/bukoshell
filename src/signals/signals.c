/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:21:58 by ccastro           #+#    #+#             */
/*   Updated: 2025/10/15 14:54:50 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <signals.h>
#include <parsing/tree.h>
#include <readline/readline.h>

volatile sig_atomic_t g_signal = 0;

static void handle_sigint_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    g_signal = 130;
    rl_done = 1;
}

static void handle_sigint_prompt(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_done = 1;
    rl_redisplay();
    g_signal = 130;
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

void set_signals_interactive(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    rl_event_hook = NULL;
    sa_int.sa_handler = handle_sigint_prompt;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void set_signals_noninteractive(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

