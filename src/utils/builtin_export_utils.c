/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:43:06 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/07 10:36:40 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
Combines consecutive arguments from a list (args), starting from a
specific index, until a stopping condition is met (token_class).
token_class[i] < 0 serves as a flag to the parser that the tokens in args
are protected and should be matched with the previous token
*/
char	*assemble_argument(char **args, int *token_class, int i)//init_args
{
	char	*arg;
	char	*combined_arg;

	combined_arg = ft_strdup(args[i]);
	while (args[++i] && token_class[i] < 0)
	{
		arg = ft_strjoin(combined_arg, args[i]);
		free(combined_arg);
		combined_arg = ft_strdup(arg);
		free(arg);
	}
	return (combined_arg);
}

/*
Increments index until it finds a positive type, which marks the beginning
of a new argument that should not be combined with the previous one.
*/
int get_next_positive(char **args, int *token_class, int i)
{
    while (args[++i])
    {
        if (token_class[i] > 0)
            return (i);
    }
    return (i);
}

/*
Restructure the arguments, combining those with negative types and
creating a new argument array
*/
char **join_args(char **args, int *token_class)
{
    int i;
    char *arg;
    char **new_args;

    i = 0;
    new_args = malloc(sizeof(char *));
    if (!new_args)
        ft_error("malloc failed", 1);
    new_args[0] = NULL;
    arg = assemble_argument(args, token_class, i);
    new_args = arg_add(arg, new_args);
    i = get_next_positive(args, token_class, i);
    while (args[i])
    {
        arg = assemble_argument(args, token_class, i);
        new_args = arg_add(arg, new_args);
        i = get_next_positive(args, token_class, i);
    }
    return (new_args);
}

char **arg_add(char *arg, char **arr)
{
    int i;
    char **new_arr;

    i = count(arr);
    new_arr = malloc(sizeof(char *) * (i + 2));
    if (new_arr == NULL)
        ft_error("malloc failed", 1);
    while(arr && arr[++i])
        new_arr[i] = ft_strdup(arr[i]);
    new_arr[i++] = ft_strdup(arg);
    new_arr[i] = NULL;
    if (arr)
        free(arr);
    free(arg);
    arr = new_arr;
    return (arr);
}
