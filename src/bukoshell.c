/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/20 03:19:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int main(void)
{
    char *line;
    t_token *head;

    while (true)
    {
        line = readline(PROMPT);
        if (ft_strncmp(line, "eof", 3) == 0)
        {
            free(line);
            break;
        }
        head = create_tokens(line);
        if (!head)
        {
            free(line);
            break;
        }
        print_tokens(head);
        free_tokens(&head);
        free(line);
    }
    return (0);
}
