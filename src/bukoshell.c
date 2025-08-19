/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/19 20:16:31 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int main(void)
{
    char *line;

    while (true)
    {
        line = readline(PROMPT);
        if (ft_strncmp(line, "eof", 3) == 0)
            break ;
        ft_printf("%s\n", line);
        free(line);
    }
    return (0);
}
