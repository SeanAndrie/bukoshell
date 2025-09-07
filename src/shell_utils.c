/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:35:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 20:28:06 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static void update_quote_state(char *str, t_quote_state *state)
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

char *handle_prompt(char *prompt)
{
	t_quote_state	state;
    char 			*line;
    char 			*temp;
    char 			*combined;

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
    return combined;
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->line)
		free(shell->line);
	if (shell->head)
		free_tokens(&shell->head);
	if (shell->root)
		free_syntax_tree(&shell->root);
	free(shell);
	clear_history();
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
	shell->token_mask = 0;
	return (shell);
}

int	start_shell(t_shell *shell)
{
	shell->line = handle_prompt(shell->line);
	if (!shell->line)
		return (EXIT_FAILURE);
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (EXIT_FAILURE);
	shell->token_mask = create_token_mask(shell->head); 
	if (DEBUG_MODE)
	{
		print_tokens(shell->head, true);
		ft_printf("\n");
	}
	// shell->root = create_syntax_tree(shell->head);
	// free_tokens(&shell->head);
	// if (!shell->root)
	// 	return (EXIT_FAILURE);
	// if (DEBUG_MODE)
	// {
	// 	ft_printf("\nSyntax Tree:\n");
	// 	print_syntax_tree(shell->root, ROOT_LEVEL);
	// 	ft_printf("\n");
	// }
	// free_syntax_tree(&shell->root);
	shell->token_mask = 0;
	free(shell->line);
	return (EXIT_SUCCESS);
}
