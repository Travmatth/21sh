/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 15:33:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/24 16:34:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GRAMMAR_H
# define GRAMMAR_H

/*
** Operator DFA definitions
*/

# define START 0
# define AMPERSAND 1
# define AND_IF 2
# define PIPE 4
# define OR_IF 4
# define SEMICOLON 5
# define DSEMI 6
# define LESS 7
# define DLESS 8
# define GREAT 9
# define DGREAT 10
# define LESSAND 11
# define GREATAND 12
# define LESSGREAT 13
# define DLESSDASH 14
# define CLOBBER 15
# define ACCEPTING(x) ((x >= 1) && (x <= 15))
# define NOT_ERR(x) (x != 16)

# define WORD 1
# define IO_HERE 2

typedef struct	s_lctx
{
	short		i;
	short		j;
	short		stop;
	short		op_state;
	short		in_word;
	char		missing;
}				t_lctx;

typedef struct	s_token
{
	int			type;
	t_buf		*value;
}				t_token;

/*
** lexer/operator_dfa.c
*/

int				can_form_op(char c);
int				next_op_state(char c, int current);

#endif
