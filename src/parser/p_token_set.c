/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/29 11:45:24 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../exec/execute.h"


void		type_of_separator(int *type_of, char **token)
{
    int i;

    i = 0;
    while (token[i])
    {
        if (token[i][0] != ' ' && token[i][0] != '\t')
            type_of[i] = -1;
        else
            type_of[i] = 1;
    i++;
    }
}

/*void filler_stokens(char **cmds, t_tokens **token, int nbr)
{
    int i;

    i = 0;
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);
    (*token)[i].type = set_token((*token)[i]);
    (*token)[i] = dollar_config((*token) +  i);

    (*token)[i] = dollar_config((*token) + i);

    
}*/
int check_unclosed_quotes(char *str)
{
    int single_quote = 0;
    int double_quote = 0;
    int i = 0;

    while (str[i]) {
        if (str[i] == '\'')
            single_quote = !single_quote;
        else if (str[i] == '\"')
            double_quote = !double_quote;
        i++;
    }
    return single_quote || double_quote;
}

//before m_export
/*void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i = 0;
    int j;
    
    printf("o comando entrou em filler_stokens\n");
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);

   
    if (!(*token)[i].tokens)
    {
        ft_error("Token split failed\n", 1);
        return;
    }

    (*token)[i].type = set_token((*token)[i]);

    if (check_unclosed_quotes(cmds[i]))
    {
        ft_error("Syntax error: unclosed quotes\n", 0);
        return;
    }

    j = 0;
    while ((*token)[i].tokens[j])// Enquanto houver tokens
    {
        printf("Token %d: %s\n", j, (*token)[i].tokens[j]);

        // Processar variáveis com dólar
        if ((*token)[i].tokens[j][0] == '$') {
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
            printf("Token after dollar config: %s\n", (*token)[i].tokens[j]);
        }

        // Verificar e processar as aspas
        if ((*token)[i].tokens[j][0] == '\'' || (*token)[i].tokens[j][0] == '\"')// Se o token começa com aspas
        {
            printf("Processing quotes for token %s\n", (*token)[i].tokens[j]);
            (*token)[i].tokens[j] = process_quotes((*token)[i].tokens[j], shell);// Processar aspas
            printf("Token after quote processing: %s\n", (*token)[i].tokens[j]);
        }
        j++;
    }

    (*token)[i].tokens[j] = NULL;
}*/

void filler_stokens(char **cmds, t_tokens **token, int nbr, t_shell *shell)
{
    int i = 0;
    int j = 0;
    
    i = 0;
    (*token)[i].nbr = count_token(cmds[i]);
    (*token)[i].pipe = nbr;
    (*token)[i].tokens = token_split(cmds[i], (*token)[i].nbr);

    if (!(*token)[i].tokens)
    {
        ft_error("Token split failed\n", 1);
        return ;
    }

    (*token)[i].type = set_token((*token)[i]);

    j = 0;
    while ((*token)[i].tokens[j])
    {
        //printf("Token %d: %s\n", j, (*token)[i].tokens[j]);

        if ((*token)[i].tokens[j][0] == '$')
            (*token)[i].tokens[j] = dollar_config((*token)[i].tokens[j], 0, shell);
        j++;
    }

    if (j > 1 && ft_strchr((*token)[i].tokens[j - 2], '='))
    {
        char *temp = ft_strjoin((*token)[i].tokens[j - 2], (*token)[i].tokens[j - 1]);
        free((*token)[i].tokens[j - 2]);
        free((*token)[i].tokens[j - 1]);
        (*token)[i].tokens[j - 2] = temp;
        (*token)[i].tokens[j - 1] = NULL;
    }
    
    (*token)[i].tokens[j] = NULL;
}


char **split_pipes(char *line)
{
    char **cmds;
    
    cmds = ft_split(line, '|');
    if (!cmds)
        ft_error("Failed to split commands by pipe\n", 1);
    return cmds;
}

void tokenize_commands(char **cmdstr, t_list **commands_list, t_shell *shell)
{
    int i;
    int j;
    char **cmds;
    t_tokens *token;
    t_exec *exec_node;
    t_list *new_node;

    cmds = split_pipes(*cmdstr);
    i = 0;
    while (cmds[i])
    {
        token = malloc(sizeof(t_tokens));
        if (!token)
            ft_error("Malloc failed in tokenize_commands\n", 1); 
        filler_stokens(&cmds[i], &token, i, shell);
        exec_node = malloc(sizeof(t_exec));
        if (!exec_node)
            ft_error("Malloc failed for exec_node", 1);
        exec_node->args = malloc(sizeof(char *) * (count_token(cmds[i]) + 1));
        if (!exec_node->args)
            ft_error("Malloc failed for exec_node->args", 1);
        j = 0;
        while (token->tokens[j])
        {
            exec_node->args[j] = ft_strdup(token->tokens[j]);
            //printf("Token %d: %s\n", j, token->tokens[j]);
            j++;
        }
        exec_node->args[j] = NULL;
        new_node = ft_lstnew(exec_node);
        if (!new_node)
            ft_error("Malloc failed for new_node", 1);
        ft_lstadd_back(commands_list, new_node);
        free_tokens(token);
        free(token->type);
        free(token);
        i++;
    }
    free_split(cmds);
}


