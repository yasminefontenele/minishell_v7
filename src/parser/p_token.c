/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:11:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/29 13:01:23 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//searches for the end of a token that starts with $
int token_dollar_end(char *str, int i)
{
    if (str[i + 1] && str[i + 1] == '$')
        return (1 + 2);
    i++;
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
			|| str[i] == '\'' || str[i] == '\"' || str[i] == '>'
			|| str[i] == '<' || str[i] == '\0')//
            return (i);
        else
            i++;
    }
    return (i);
}

int token_word_end(char *str, int i)
{
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
            || str[i] == '>' || str[i] == '<' || str[i] == '\0')
            return (i);
        else if ((str[i] == '\'' || str[i] == '\"')
            && (count_backslash(str, i) % 2 == 0))
            return (i);
        else
            i++;
    }
    return (i);
}

int token_quotes_end(char *str, int i)
{
    int j;

    j = i + 1;
    while (str[i])
    {
        if (str[j] == str[i] && str[i] == '\'')
            return (j + 1);
        if (str[j] == str[i] && (count_backslash(str, j) % 2 == 0))
            return (j + 1);
        j++;
    }
    return (-1);
}

int token_redir_end(char *str, int i)
{
    int j;

    j = i;
    while (str[i])
    {
        if (str[j] == str[i])
            i++;
        else
            return (i);
    }
    return (i);
}
//function before changing m_export
/*char *parse_next_token(char *line, int reset)
{
    printf("entrou em parse_next_token\n");
    static int current_pos = 0;//static variable to keep track of the current position in the line
    char *token;//variable to store the token
    int end;//variable to store the end of the token
    int initial_offset;//variable to store the initial offset of the token

    token = NULL;
    initial_offset = 0;
    if (reset == 0)//if reset is 0, reset the current position to 0
        current_pos = 0;
    if (current_pos == 0)//if the current position is 0, skip the leading spaces
        initial_offset = 0;
    while (line[current_pos])//iterate over the line
    {
        current_pos = skip_space(line, current_pos);//skip spaces
        if (line[current_pos])//if the current position is not null
        {
            end = get_end(line, current_pos);//get the end of the token
            if (end != -1)//if the end is not -1
                token = extract_substring(line, current_pos - initial_offset, end);//extract the token
            current_pos = end;
            return (token);
        }
    }
    return (token);
}*/

char *parse_next_token(char *line, int reset)
{
    static int current_pos;
    char *token;
    int start;
    int end;
    char quote_char;

    token = NULL;
    if (reset == 0)
        current_pos = 0;
    while (line[current_pos]) {
        current_pos = skip_space(line, current_pos);
        if (line[current_pos]) {
            if (line[current_pos] == '\"')
            {
                quote_char = line[current_pos];
                current_pos++;
                start = current_pos;
                while (line[current_pos] && line[current_pos] != quote_char)
                    current_pos++;
                if (line[current_pos] == quote_char)
                {
                    end = current_pos;
                    current_pos++;
                } 
                else
                    end = -1;
            } 
            else
            {
                start = current_pos;
                while (line[current_pos] && line[current_pos] != ' ' && line[current_pos] != '\t' && line[current_pos] != '\"')
                    current_pos++;
                end = current_pos;
            }
            if (end != -1)
                token = extract_substring(line, start, end);
            current_pos = end;
            return token;
        }
    }
    return token;
}




