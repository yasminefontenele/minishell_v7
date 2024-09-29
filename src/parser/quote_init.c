/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:49:12 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/04 08:49:16 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_quote quote_init(void)
{
    t_quote quote;

    quote.single_quote = false;
    quote.double_quote = false;
    return (quote);
}