/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_form.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:57:00 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 09:23:51 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void free_form(char ***str)
{
    int i;

	i = 0;
    while (str && str[0] && str[0][i])
    {
        free(str[0][i]);
		i++;
    }
	if (str)
	{
    	free(str[0]);
   	 	*str = NULL;
	}
}

int	form_len(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	**extend_form(char **insd, char *nstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	len = form_len(insd);
	if (!nstr || !(out = malloc(sizeof(char *) * (len + 2))))
		return (insd);
	while (++i < len)
	{
		if (!(out[i] = ft_strdup(insd[i])))
		{
			free_form(&out);
			return (insd);
		}
	}
	if (!(out[i] = ft_strdup(nstr)))
	{
		free_form(&out);
		return (insd);
	}
	out[i + 1]= NULL;
	free_form(&insd);
	return (out);
}

char	**replace_form(char **big, char **small, int nbr)
{
	char	**add;
	int		i;
	int		j;
	int		k;

	i = -1;
	j = -1;
	k = nbr + 1;
	if (!big || !*big || nbr < 0 || nbr >= form_len(big))
		return (NULL);
	add = ft_calloc(form_len(big) + form_len(small), sizeof(char *));
	if (!add)
		return (NULL);
	while (++i < nbr)
		add[i] = ft_strdup(big[i]);
	while(++j < form_len(small))
		add[i++] = ft_strdup(small[j]);
	while(k++ < form_len(big))
		add[i++] = ft_strdup(big[k]);
	free_form(&big);
	return (add);
}

char	**dupl_form(char **f)
{
	char	**out;
	int		row_nbr;
	int		i;

	i = 0;
	row_nbr = form_len(f);
	out = malloc(sizeof(char *) * (row_nbr + 1));
	if(!out)
		return (NULL);
	while (f[i])
	{
		out[i] = ft_strdup(f[i]);
		if(!out[i])
		{
			free_form(&out);
			return (NULL);
		}
		i++;
	}
	out[i] = NULL;
	return (out);
}
