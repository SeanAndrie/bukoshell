/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:08:28 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 00:03:25 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <dirent.h>
#include <expand.h>
#include <libft.h>

static void	backtrack(t_glob *g)
{
	g->p = g->star;
	g->s_back++;
	g->s = g->s_back;
}

static void	advance(t_glob *g)
{
	g->p++;
	g->s++;
}

t_bool	is_matching_pattern(char *pattern, char *str)
{
	t_glob	g;

	if ((*str == '.' && *pattern != '.') || !ft_strcmp(str, ".")
		|| !ft_strcmp(str, ".."))
		return (FALSE);
	g = (t_glob){str, pattern, NULL, NULL};
	while (*g.s)
	{
		if (*g.p == '*')
		{
			while (*g.p == '*')
				g.p++;
			g.star = g.p;
			g.s_back = g.s;
		}
		else if (*g.s == *g.p)
			advance(&g);
		else if (g.star)
			backtrack(&g);
		else
			return (FALSE);
	}
	while (*g.p == '*')
		g.p++;
	return (*g.p == '\0');
}

static int	partition(char **arr, int low, int high)
{
	int		i;
	int		j;
	char	*temp;
	char	*pivot;

	pivot = arr[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (ft_strcmp(arr[j], pivot) <= 0)
		{
			i++;
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
		j++;
	}
	temp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = temp;
	return (i + 1);
}

void	quick_sort(char **arr, int low, int high)
{
	int	pivot_i;

	if (low < high)
	{
		pivot_i = partition(arr, low, high);
		quick_sort(arr, low, pivot_i - 1);
		quick_sort(arr, pivot_i + 1, high);
	}
}
