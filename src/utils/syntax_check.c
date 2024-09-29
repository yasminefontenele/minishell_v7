/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 15:03:59 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/05 19:37:35 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//verifies if the token is a redirection (>, >>, <, <<)
int syntax_redirections(t_tokens *tokens)
{
    int i;
    int j;

    i = -1;
    if (++i < tokens->pipe)
    {
        j = -1;
        while (tokens[i].tokens[++j])
        {
            if (is_redirection(tokens[i].type[j] != 0))
            {
                if (tokens[i].tokens[j + 1])
                {
                    if (error_redirection(tokens[i].type[j + 1]) == 1)
                        return (0);
                }
                else
                {
                    ft_error("syntax error near unexpected token", 0);
                    return (0);
                }
            }
        }
    }
    return (1);
}

//verifies if the token is empty
int syntax_grammar(char **commands, t_tokens *tokens)
{
    int i;

    i = -1;
    if (commands[++i])
    {
        if (is_empty(commands[i]) == 1)
        {
            ft_error("syntax error near unexpected token", 0);
            return (0);
        }
    }
    i = -1;
    if (syntax_redirections(tokens) == 0)
        return (0);
    return (1);
}