/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_dfa.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 14:37:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 22:42:27 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_DFA_H
# define OPERATOR_DFA_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

/*
** Operator DFA symbols used to match input to specific bash operator
*/

enum	e_parse_ops
{
	EOI = -1,
	START = 0,
	AMPERSAND = 1,
	AND_IF = 2,
	PIPE = 3,
	OR_IF = 4,
	SEMICOLON = 5,
	DSEMI = 6,
	LESS = 7,
	DLESS = 8,
	GREAT = 9,
	DGREAT = 10,
	GREATAND = 11,
	LESSAND = 12,
	LESSGREAT = 13,
	DLESSDASH = 14,
	CLOBBER = 15,
	L_PAREN = 16,
	R_PAREN = 17,
	LEXER_WORD = 18,
	IO_NUMBER = 19
};

/*
** Operator DFA defines used to help match input to specific bash operator
*/

# define OP_TOKEN_CONVERSIONS 19
# define OP_DFA_ACCEPTING(x) ((x >= 1) && (x <= 17))
# define OP_DFA_NOT_ERR(x) (x != 20)
#endif
