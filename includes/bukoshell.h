/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/23 07:31:23 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <stdio.h>
# include <parsing.h>
# include <readline/history.h>
# include <readline/readline.h>

# define PROMPT "bukoshell> "

// Debugging
void	print_tokens(t_token *head);

#endif
