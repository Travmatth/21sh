/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_dfa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 17:21:52 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/20 16:22:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static short	g_operator_transition_table[][17] =
{
	[1, 1, 2, 7, 16, 16], // 0, start
	[2, 16, 16, 16, 16, 16], // 1
	[16, 16, 16, 16, 16, 16], // 2
	[16, 4, 16, 16, 16, 16], // 3
	[16, 16, 16, 16, 16, 16], // 4
	[16, 16, 6, 16, 16, 16], // 5
	[16, 16, 16, 16, 16, 16], // 6
	[11, 16, 16, 8, 16, 13], // 7
	[16, 16, 16, 16, 14, 16], // 8
	[12, 15, 16, 16, 16, 10], // 9
	[16, 16, 16, 16, 16, 16], // 10
	[16, 16, 16, 16, 16, 16] // 11
	[16, 16, 16, 16, 16, 16], // 12
	[16, 16, 16, 16, 16, 16], // 13
	[16, 16, 16, 16, 16, 16], // 14
	[16, 16, 16, 16, 16, 16], // 15
	[16, 16, 16, 16, 16, 16], // 16, error
}

int				process_operator(c, token, ctx)
{
	/*
	** 2. If the previous character was used as part of
	** an operator and the current character is not
	** quoted and can be used with the current
	** characters to form an operator,
	** it shall be used as part of that (operator) token.
	*/
	/*
	** 3. If the previous character was used as part of an
	** operator and the current character cannot be used
	** with the current characters to form an operator, the
	** operator containing the previous character shall be delimited.
	*/
	/*
	** 6: If the current character is not quoted and can be used as the
	** first character of a new operator, the current token (if any)
	** shall be delimited. The current character shall be used as the
	** beginning of the next (operator) token.
	*/
}