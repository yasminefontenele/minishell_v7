/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backslash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 20:57:57 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/07 10:34:15 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int count_backslash(char *line, int i)
{
    int count;

    count = 0;
    if(i == 0)
        return (0);
    while (i-- >= 0)
    {
        if (i >= 0 && line[i] == '\\')
            count++;
        else
            break;
    }
    return (count);
}

//Tcheck if the backslash is valid
int valid_backslash(char **tokens)
{
    int i;
    int j;
    t_quote q;

    i = -1;
    if(tokens[++i])
    {
        j = -1;
        q = quote_init();//initialize the quote structure
        while (tokens[i][++j])
        {
            if ((tokens[i][j] == '\\' && q.double_quote == false) && q.single_quote == false)
                return (0);
            if (tokens[i][j] == '\'' || tokens[i][j] == '\"')//if the token is a quote
                q = quote_value(tokens[i][j], q);
        }
    }
    return (1);
}

void    join_backslash(char **path)
{
    int i;
    char    *tmp;
    
    i = -1;
    if (path == NULL)
        return ;
    while (path [++i])
    {
        tmp = ft_strjoin(path[i], "/");
        free(path[i]);
        path[i] = tmp;
    }
}