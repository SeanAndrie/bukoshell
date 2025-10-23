/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ext.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:05:40 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/23 14:26:34 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <signals.h>
#include <sys/wait.h>
#include <execute/exec.h>

void handle_signal(pid_t pid, int *status)
{
    int sig;

    if (waitpid(pid, status, 0) == -1)
    {
        perror("waitpid");
        *status = 1;
        return ;
    }
    if (WIFEXITED(*status))
        *status = WEXITSTATUS(*status);
    else if (WIFSIGNALED(*status))
    {
        sig = WTERMSIG(*status);
        if (sig == SIGINT)
            ft_printf("\n");
        else if (sig == SIGQUIT)
            ft_printf("Quit (core dumped)\n");
        g_signal = 128 + sig;
        *status = 128 + sig;
    }
}

