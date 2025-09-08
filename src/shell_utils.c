/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:35:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/08 12:23:28 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static bool	has_unbalanced_quotes(const char *str)
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

static char	*handle_prompt(char *prompt)
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

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	handle_signals();
	shell->status = 0;
	shell->line = NULL;
	shell->head = NULL;
	shell->root = NULL;
	shell->prompt_mask = 0;
	return (shell);
}

int start_shell(t_shell *shell)
{
    char *new_line;

    if (has_unbalanced_quotes(shell->line))
    {
        new_line = handle_prompt(shell->line);
        if (!new_line)
            return (free(shell->line), EXIT_FAILURE);
        shell->line = new_line;
    }
    shell->head = create_tokens(shell->line);
    if (!shell->head)
        return (free_shell(shell, false), EXIT_FAILURE);
    shell->prompt_mask = create_prompt_mask(shell->head);
    if (DEBUG_MODE)
	{
        print_tokens(shell->head, true);
		ft_printf("\n");
	}
    if (!are_valid_tokens(shell->head))
        return (free_shell(shell, false), 2);
    // TODO: Add AST creation here ...  
    // shell->root = create_syntax_tree(shell->head);
	free_shell(shell, false);
    shell->prompt_mask = 0;
    return (shell->status);
}
