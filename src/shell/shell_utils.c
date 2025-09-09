/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:35:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:18:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

bool	has_unbalanced_quotes(const char *str)
{
	t_quote_state	state;

	state = (t_quote_state){false, false};
	while (*str)
	{
		if (*str == '\'' && !(state.in_double))
			state.in_single = !(state.in_single);
		else if (*str == '"' && !(state.in_single))
			state.in_double = !(state.in_double);
		str++;
	}
	return (state.in_single || state.in_double);
}

static void	update_quote_state(char *str, t_quote_state *state)
{
	while (*str)
	{
		if (*str == '\'' && !(state->in_double))
			state->in_single = !(state->in_single);
		else if (*str == '"' && !(state->in_single))
			state->in_double = !(state->in_double);
		str++;
	}
}

char	*handle_unclosed_prompt(char *prompt)
{
	t_quote_state	state;
	char			*line;
	char			*temp;
	char			*combined;

	state = (t_quote_state){false, false};
	combined = ft_strdup(prompt);
	if (!combined)
		return (free(prompt), NULL);
	free(prompt);
	update_quote_state(combined, &state);
	while (state.in_single || state.in_double)
	{
		line = readline(PS2);
		if (!line)
			return (free(combined), NULL);
		update_quote_state(line, &state);
		temp = ft_vstrjoin(2, "\n", combined, line);
		free(line);
		if (!temp)
			return (free(combined), NULL);
		free(combined);
		combined = temp;
	}
	return (combined);
}