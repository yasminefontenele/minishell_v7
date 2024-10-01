/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:11:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/01 13:18:09 by yfontene         ###   ########.fr       */
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

/*char *parse_next_token(char *line, int reset)
{
    static int current_pos;
    char *token;
    int start;
    int end;
    char quote_char;

    token = NULL;
    if (reset == 0)
        current_pos = 0;

    //printf("Starting parse_next_token at position: %d\n", current_pos);
    
    while (line[current_pos])
    {
        current_pos = skip_space(line, current_pos);
        //printf("Current char after skip_space: '%c'\n", line[current_pos]);
        if (line[current_pos])
        {
            if (line[current_pos] == '>' || line[current_pos] == '<')
            {
                //printf("Redirection token found: %c\n", line[current_pos]);
                start = current_pos;
                if (line[current_pos + 1] == '>') // Caso de ">>" ou "<<"
                    current_pos += 2;
                else
                    current_pos++;
                end = current_pos;
                return extract_substring(line, start, end); // Extrai o redirecionamento como token
            }
            if (line[current_pos] == '\"')
            {
                quote_char = line[current_pos];
                current_pos++;
                start = current_pos;
                //printf("Entering quotes: %c\n", quote_char);
                while (line[current_pos] && line[current_pos] != quote_char)
                    current_pos++;
                if (line[current_pos] == quote_char)
                {
                    end = current_pos;
                    current_pos++;
                    //printf("Closing quotes at position: %d\n", current_pos);
                } 
                else
                    end = -1;
            } 
            else
            {
                start = current_pos;
                while (line[current_pos] && line[current_pos] != ' ' && line[current_pos] != '\t' &&
                       line[current_pos] != '\"' && line[current_pos] != '\'' &&
                       line[current_pos] != '>' && line[current_pos] != '<')
                {
                    current_pos++;
                }
                end = current_pos;
            }
            if (end != -1)
            {
                token = extract_substring(line, start, end);
                //printf("Captured token: '%s'\n", token);
                return token;
            }
                
            current_pos = end;
            return token;
        }
    }
    return token;
}*/

char *parse_next_token(char *line, int reset)
{
    static int current_pos;
    int start = 0;
    int end = 0;
    char quote_char;

    if (reset == 0)
        current_pos = 0;

    while (line[current_pos])
    {
        current_pos = skip_space(line, current_pos);
        if (line[current_pos] == '\"' || line[current_pos] == '\'')
        {
            quote_char = line[current_pos++];
            start = current_pos;
            while (line[current_pos] && line[current_pos] != quote_char)
                current_pos++;

            end = current_pos++;
            return extract_substring(line, start - 1, end);
        }
        else if (line[current_pos] == '>' || line[current_pos] == '<')
        {
            start = current_pos++;
            if (line[current_pos] == line[start])
                current_pos++;
            end = current_pos;
            return extract_substring(line, start, end);
        }
        else
        {
            start = current_pos;
            while (line[current_pos] && !isspace(line[current_pos]) && line[current_pos] != '>' && line[current_pos] != '<')
                current_pos++;
            end = current_pos;
            return extract_substring(line, start, end);
        }
    }
    return NULL;
}




