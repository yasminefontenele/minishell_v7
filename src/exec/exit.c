/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:25 by emencova          #+#    #+#             */
/*   Updated: 2024/09/24 13:21:57 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int m_exit(t_shell *shell, t_list *cmd_node, int *leave)
{
    t_exec *cmd = (t_exec *)cmd_node->content;
    char *arg;
    int exit_value;
    int i;

  //  printf("Inside m_exit, exec in is - %d, exec out is - %d, path is - %s args are - %s\n",cmd->in, cmd->out, cmd->path, *cmd->args);
    *leave = !shell->cmds->next;
    i = 0; 
    if (!cmd->args || !cmd->args[1])
    {
        //printf("Exiting with status 0\n");
        exit(0);
    }
    if (cmd->args[2])
    {
        *leave = 0;
        return (1);
    }
    arg = cmd->args[1];
    while (arg[i] != '\0')
    {
        if (!isdigit(arg[i]) && !(i == 0 && (arg[i] == '-' || arg[i] == '+')))
        {
          //  fprintf(stderr, "Invalid exit argument\n");
            return (1); // Indicate failure
        }
        i++;
    }
    exit_value = atoi(arg) % 256;
    if (exit_value < 0)
        exit_value += 256;

    //printf("Exiting with status %d\n", exit_value);
    exit(exit_value);
}