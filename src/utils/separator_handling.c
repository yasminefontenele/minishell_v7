/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 16:01:18 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/28 14:31:39 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_quote quote_value(char type_quote, t_quote status_quote)
{
    t_quote quote;

    quote.double_quote = status_quote.double_quote;
    quote.single_quote = status_quote.single_quote;
    if (status_quote.double_quote == false && status_quote.single_quote == false)
    {
        if (type_quote == 39)
            quote.single_quote = true;
        else
            quote.double_quote = true;
    }
    else if (status_quote.double_quote == true && type_quote == 34)
        quote.double_quote = false;
    else if (status_quote.single_quote == true && type_quote == 39)
        quote.single_quote = false;
    return (quote);
}

int count_separator(char *line, char separator)
{
    int i;
    int count;
    t_quote quote;

    i = -1;
    count = 0;
    quote = quote_init();
    while (line[++i])
    {
        if (line[i] == 34 || line[i] == 39)
        {
            if (count_backslash(line, i) % 2 == 0)
                quote = quote_value(line[i], quote);
        }
        if (separator == ';' && line[i] == ';' && line[i + 1] == ';')
            ft_error("syntax error near unexpected token", 0);
        else if (separator == '|' && line[i] == '|' && line[i + 1] == '|')
            ft_error("syntax error near unexpected token", 0);
        else if (line[i] == separator)
            if (count_backslash(line, i) % 2 == 0)
                if(!quote.single_quote && !quote.double_quote)
                    count++;
    }
    return (count);
}

t_separator position_separator(char *line, char separator)
{
    t_quote quote;
    int i;
    int count;
    t_separator sep;

    i = -1;
    count = 0;
    quote = quote_init();
    sep.separator_index = NULL;
    sep.nbr_separator = count_separator(line, separator);
    sep.separator_index = malloc(sizeof(int) * (sep.nbr_separator + 1));
    if (!sep.separator_index)
        ft_error("malloc failed", 1);
    while (line[++i])
    {
        if ((line[i] == 34 || line[i] == 39) && (count_backslash(line, i) % 2 == 0))
            quote = quote_value(line[i], quote);
        else if (line[i] == separator && (count_backslash(line, i) % 2 == 0))
            if (!quote.single_quote && !quote.double_quote)
                sep.separator_index[count++] = i;
    }
    return (sep);
}