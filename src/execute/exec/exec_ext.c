/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ext.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:05:40 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/26 13:40:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <signals.h>
#include <sys/wait.h>
#include <execute/exec.h>

static void print_argv(char **argv)
{
    size_t  i;

    if (!argv || !*argv)
        return ;
    i = 0;
    while (argv[i])
    {
        if (argv[i])
            ft_printf(" ");
        ft_printf("%s", argv[i]);
        i++;
    }
    ft_printf("\n");
}

void handle_signal(pid_t pid, int *status, char **argv)
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
        {
            ft_printf("Quit\t\t\t\t(core dumped)");
            print_argv(argv);
        }
        g_signal = 128 + sig;
        *status = 128 + sig;
    }
}
