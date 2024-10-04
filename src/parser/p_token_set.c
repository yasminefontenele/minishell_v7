/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:47:09 by yfontene          #+#    #+#             */
/*   Updated: 2024/10/04 22:17:50 by eliskam          ###   ########.fr       */
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
    int i;
    int j;
    
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
        //printf("Token original %d: %s\n", j, (*token)[i].tokens[j]);
        (*token)[i].tokens[j] = remove_quotes((*token)[i].tokens[j]);
        //printf("Token após remover aspas %d: %s\n", j, (*token)[i].tokens[j]);
        if (get_token_type((*token)[i].tokens[j], ft_strlen((*token)[i].tokens[j])))
            printf("Redirection token found: %s\n", (*token)[i].tokens[j]);
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

char **lst_to_array(t_list *list)
{
    int size = ft_lstsize(list);
    char **array = malloc(sizeof(char *) * (size + 1));
    int i = 0;

    if (!array)
        return NULL;

    while (list)
    {
        array[i] = ft_strdup((char *)list->content);
        list = list->next;
        i++;
    }
    array[i] = NULL;
    return array;
}


char **split_pipes(char *line)
{
    char **cmds;
    int i = 0;
    int start = 0;
    bool in_quotes = false;
    char quote_type = '\0';
    t_list *cmd_list = NULL;

    if (line[i] == '|')
    {
        write(STDERR_FILENO, "Error: Misuse of pipe\n", 23);
        g_env.exit_status = 2;
        return (NULL);
    }
    while (line[i])
    {
        if ((line[i] == '"' || line[i] == '\'') && (quote_type == '\0' || quote_type == line[i]))
        {
            if (!in_quotes)
                quote_type = line[i];
            in_quotes = !in_quotes;
            if (!in_quotes)
                quote_type = '\0';
        }
        if (line[i] == '|' && !in_quotes)
        {
            char *cmd = ft_substr(line, start, i - start);
            ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
            start = i + 1;
        }
        i++;
    }
    if (start < i)
    {
        char *cmd = ft_substr(line, start, i - start);
        ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
    }
    cmds = lst_to_array(cmd_list);
    ft_lstclear(&cmd_list, free);
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
    if (!cmds)
        return;
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
        exec_node->path = NULL;
        exec_node->in = 0;
        exec_node->out = 1;
        if (!(exec_node)->args[0] || ft_strlen((exec_node)->args[0]) == 0 || ft_str_is_space((exec_node)->args[0]))
            return ; 
        if (!new_node)
            ft_error("Malloc failed for new_node", 1);
        ft_lstadd_back(commands_list, new_node);
        if (is_invalid_var_assignment(exec_node->args[0]))
            return;
        free_tokens(token);
        free(token->type);
        free(token);
        i++;
    }
    free_split(cmds);
}

