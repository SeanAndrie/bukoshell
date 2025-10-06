/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompts.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 02:57:36 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/03 19:11:32 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPTS_H
# define PROMPTS_H

# if defined(__linux__)
#  define PS1 "\e[1mbukoshell\e[m ❯ "
#  define PS2 "∙ "
#  define PATH_MAX 4096
# elif defined(__APPLE__)
#  define PS1 "\e[1mbukoshell\e[m % "
#  define PS2 "> "
# endif

#endif

