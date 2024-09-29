/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:48:35 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/29 11:49:07 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int skip_space(char *line, int i)
{
    while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else
			return (i);
	}
	return (i);
}


int	get_end(char *line, int i)
{
	if ((line[i] == '>' || line[i] == '<')
		&& count_backslash(line, i) % 2 == 0)
        i = token_redir_end(line, i);
    else if (line[i] == '$' && count_backslash(line, i) % 2 == 0)
        i = token_dollar_end(line, i);
    else if (line[i] == '\'' || line[i] == '\"')
    {
        i = token_quotes_end(line, i);
        if (i == -1)
            ft_error("syntax error, unclosed quotes.", 0);
    }
    else
        i = token_word_end(line, i);
    return (i);
}

int count_token(char *line)
{
    int     i;
    int     count;

    i = 0;
    count = 0;
    while (line[i])
    {
        if (line[i])
            i = skip_space(line, i);
        if (line[i])
        {
            i = get_end(line, i);
            if (i == -1)
                return (count);
            count++;
        }
        else
            return (count);
        i++;
    }
    return (count);
}

/*char	*extract_substring(char const *s, unsigned int start, size_t len)
{
	char			*sub;
	int				i;

	i = 0;
	sub = malloc(sizeof(char) * (len - start + 1));
	if (sub == NULL)
		ft_error("malloc error in extract_substring", 1);
	while (start < len)
	{
		sub[i] = s[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}*/

char *extract_substring(char const *s, unsigned int start, size_t end)
{
    char *sub;
    size_t i;
    size_t substring_len;

    substring_len = end - start;
    if (start >= end || substring_len == 0)
        return NULL;
    sub = malloc(sizeof(char) * (substring_len + 1));
    if (sub == NULL)
        ft_error("malloc error in extract_substring", 1);
    i = 0;
    while(i < substring_len)
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}

