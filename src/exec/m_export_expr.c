/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_export_expr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 08:08:44 by emencova          #+#    #+#             */
/*   Updated: 2024/10/01 13:19:15 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../minishell.h"
#include "execute.h"


#include <ctype.h>

int is_valid_env_var(const char *var_name)
{
    int i = 0;


    while (isspace(var_name[i]))
        i++;
    if (var_name[i] == '\0')
        return 0;
    while (var_name[i] != '\0')
        i++;
    while (isspace(var_name[i - 1]))
        i--;
    return i > 0;
}

void print_all_variables(char **keys)
{
    int i;
    
    i = 0;
    while (keys[i])
    {
        printf("declare -x %s\n", keys[i]);
        i++;
    }
}

/*char *remove_quotes(char *str)
{
    size_t len;
    char *new_str;

    len = ft_strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        new_str = malloc(len - 1);
        ft_strlcpy(new_str, str + 1, len - 2);//
        new_str[len - 2] = '\0';
        free(str);
        return new_str;
    }
    return str;
}*/

char *remove_quotes(char *arg)
{
    char *result;
    int i;
    int j;

    if (!arg)
        return NULL;
    result = malloc(strlen(arg) + 1);
    if (!result)
        return NULL;
    i = 0;
    j = 0;
    while (arg[i])
    {
        if (arg[i] != '"' && arg[i] != '\'')
            result[j++] = arg[i];
        i++;
    }
    result[j] = '\0';

    return result;
}



//last version before the correction
/*void split_var_value(char *arg, char **var, char **value)
{
    printf("Argumento recebido em split_var_value: '%s'\n", arg);
    char *eq_pos;

    eq_pos = strchr(arg, '=');  // Encontra a posição do '='
    
    if (!eq_pos)
    {
        // Se não houver '=', então não é uma atribuição válida de variável
        *var = NULL;
        *value = NULL;
        return;
    }
    // Dividir a string antes do '=' para obter o nome da variável
    *var = ft_strndup(arg, eq_pos - arg);  // Copiar a parte antes do '=' para 'var'
 
    printf("aqui var recebe certo var='%s', value='%s'\n", *var, *value);
    // Se houver algo após o '=', será o valor
    if (*(eq_pos + 1) != '\0')
    {
        printf("Valor após '=': %s\n", eq_pos + 1);
        // O valor começa após o '='
        *value = ft_strdup(eq_pos + 1);
        printf("porque aqui da erro var='%s', value='%s'\n", *var, *value);
        // Agora remover aspas ao redor, se houver
        if ((*value)[0] == '"' && (*value)[strlen(*value) - 1] == '"')
        {
            (*value)[strlen(*value) - 1] = '\0';  // Remove a última aspa
            memmove(*value, *value + 1, strlen(*value));  // Remove a primeira aspa
        }
        else if ((*value)[0] == '\'' && (*value)[strlen(*value) - 1] == '\'')
        {
            (*value)[strlen(*value) - 1] = '\0';  // Remove a última aspa simples
            memmove(*value, *value + 1, strlen(*value));  // Remove a primeira aspa simples
        }
    }
    else
    {
        // Caso não haja valor após o '=', o valor é uma string vazia
        *value = ft_strdup("");
        printf("Sem valor após '=', definindo value como vazio: var='%s', value='%s'\n", *var, *value);
    }
     printf("apos split: var='%s', value='%s'\n", *var, *value);

}*/

void split_var_value(char *arg, char **var, char **value)
{
    char *eq_pos;
    size_t len;

    eq_pos = strchr(arg, '=');
    if (!eq_pos)
    {
        *var = NULL;
        *value = NULL;
        return;
    }
    *var = ft_strndup(arg, eq_pos - arg);
    if (*(eq_pos + 1) != '\0')
    {
        *value = ft_strdup(eq_pos + 1);


        len = ft_strlen(*value);
        if (len >= 2 && ((*value)[0] == '"' && (*value)[len - 1] == '"'))
        {
            (*value)[len - 1] = '\0';
            ft_memmove(*value, *value + 1, len - 1);
        }
        else if (len >= 2 && ((*value)[0] == '\'' && (*value)[len - 1] == '\''))
        {
            (*value)[len - 1] = '\0';
            ft_memmove(*value, *value + 1, len - 1);
        }
    }
    else
        *value = ft_strdup("");
}

int m_export(t_shell *shell)
{
    int i;
    char **av;
    char *var_name;
    char *value;
    char *equals_sign;
    char *quoted_value;
    char *temp;
    int index;
    char *new_entry;
    
    av = ((t_exec *)shell->cmds->content)->args;
    i = 1;

    if (!av[1])
    {
        print_all_variables(shell->keys);
        return 0;
    }

    while (av[i])
    {
        equals_sign = ft_strchr(av[i], '=');
        if (equals_sign != NULL)
        {
            split_var_value(av[i], &var_name, &value);
            if (var_name && is_valid_env_var(var_name))
            {
                if (value && ft_strchr(value, ' '))
                {
                    quoted_value = ft_strdup(value);


                    index = find_key_idx(shell->keys, var_name);
                    if (index != -1)
                    {
                        free(shell->keys[index]);
                        shell->keys[index] = ft_strjoin(var_name, "=");
                        shell->keys[index] = ft_strjoin(shell->keys[index], quoted_value);
                    }
                    else
                    {
                        new_entry = ft_strjoin(var_name, "=");
                        temp = new_entry;
                        new_entry = ft_strjoin(new_entry, quoted_value);
                        free(temp);
                        shell->keys = extend_form(shell->keys, new_entry);
                        free(new_entry);
                    }
                    free(quoted_value);
                }
                else
                {
                    index = find_key_idx(shell->keys, var_name);
                    if (index != -1)
                    {
                        free(shell->keys[index]);
                        shell->keys[index] = ft_strdup(av[i]);
                    }
                    else
                        shell->keys = extend_form(shell->keys, av[i]);
                }
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
            free(var_name);
            free(value);
        }
        else
        {
            if (is_valid_env_var(av[i]))
            {
                index = find_key_idx(shell->keys, av[i]);
                if (index == -1)
                    shell->keys = extend_form(shell->keys, av[i]);
                else
                    value = av[i];
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
        }
        i++;
    }
    return 1;
}

int m_expr(char **args)
{
    int num1, num2;
    char op;

    if (!args[1] || !args[2] || !args[3])
        return 1;
    num1 = atoi(args[1]);
    op = args[2][0];
    num2 = atoi(args[3]);
    if (op == '+')
        printf("%d\n", num1 + num2);
    else if (op == '-')
        printf("%d\n", num1 - num2);
    else if (op == '*')
        printf("%d\n", num1 * num2);
    else if (op == '/')
    {
        if (num2 == 0)
        {
            printf("expr: divisão por zero\n");
            return 1;
        }
        printf("%d\n", num1 / num2);
    }
    else
    {
        printf("expr: operador inválido\n");
        return 1;
    }
    return 0;
}

