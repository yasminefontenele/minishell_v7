/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:25:30 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/23 18:26:38 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    process_pipeline(char *line, t_shell *shell)
{
    int         i;
    int         error;
    t_separator sep;
    t_list  *cmds_list;

    i = 0;
    error = 0;
    sep = position_separator(line, '|');
    if (sep.nbr_separator == 0)
        sep.content = ft_split(line, '\0');
    else
        sep.content = separator_split(line, sep.separator_index, sep.nbr_separator);
    free(sep.separator_index);
    sep.separator_index = NULL;
    while (sep.content[i])
    {
        if (sep.content[i][0] == ';' || sep.content[i][0] == '|')
            error = 1;
        i++;
    }
    if (error == 0)
        tokenize_commands(sep.content, &cmds_list, shell);
    free(sep.content);
}

void	process_command_line(char *line, t_shell *shell)
{
    t_separator semicolon_info;
    
    if (ft_strlen(line) == 0)//if the line is empty, it will be ignored
    {
        free(line);
        line = NULL;
        return ;
    }
    semicolon_info = position_separator(line, ';');
    if (semicolon_info.nbr_separator == 0)//if there is no semicolon, the line is processed as a pipeline
        process_pipeline(line, shell);
    else if (semicolon_info.nbr_separator == 1)//if there is a semicolon, the line is split into two commands
    {
        semicolon_info.content = ft_split(line, '\0');
        process_pipeline(semicolon_info.content[0], shell);
        free(semicolon_info.content[0]);
        semicolon_info.content[0] = NULL;
        free(semicolon_info.content);
    }
    else
        ft_error("this shell does not support unspecified special characters like ';'", 1);
    free(semicolon_info.separator_index);
    free(line);
    semicolon_info.separator_index = NULL;
    line = NULL;
}
