/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_word.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 14:49:59 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 18:46:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef RESERVED_WORD_H
# define RESERVED_WORD_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

/*
** Reserved word DFA symbols used to match given tokens
** to specific reserved words
*/

enum	e_reserved_ops
{
	RESERVED_IF,
	RESERVED_THEN,
	RESERVED_ELSE,
	RESERVED_ELIF,
	RESERVED_FI,
	RESERVED_DO,
	RESERVED_DONE,
	RESERVED_CASE,
	RESERVED_ESAC,
	RESERVED_WHILE,
	RESERVED_UNTIL,
	RESERVED_FOR,
	RESERVED_LBRACE,
	RESERVED_RBRACE,
	RESERVED_BANG,
	RESERVED_IN
};

#define RESERVED_WORD_CONVERSIONS 16
#define RESERVED_LEX_ERR "Lexical Error: Unimplemented reserved word: %s"

/*
** Reserved word struct used to translate given token to
** specific reserved word
*/

typedef struct	s_reserved_conversion
{
	char	*reserved;
	int		symbol;
}				t_reserved_conversion;

/*
** src/lexical_analysis/reserved_dfa.c
*/

struct			s_token;
int				process_reserved(struct s_token *token);
#endif
