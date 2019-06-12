/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_dfa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 17:21:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 17:26:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** when attempting to determine whether a given character starts or is a part
** of an operator (and identify said operator), we use a dfa to say, whether
** given a character and a state, this character could be used to form a
** valid operator. g_operator_char_table is used to translate an ascii
** character to a integer that can be used in the operator dfa
*/

static short	g_operator_char_table[128] =
{
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 0, 8,
	6, 7, 8, 8, 8, 4, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 2, 3, 8, 5, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 1, 8, 8, 8
};

/*
** To transform a sequence of characters into an identifiable operator,
** we start at state 0 and character 0. g_operator_char_table is used to
** translate the character into it's appropriate category, and we look up
** the category in the current state. If the next state specified by the
** category index references another valid state, the character may be
** combined with the existing operator characters to form a new operator,
** and this new state becomes the current state. If the state referenced by
** the current state and the current category index points to the error state,
** this character cannot be combined with the current operator to form a
** valid operator.
**
**      &   |   ;   <   -   >  (    )   other
**  0 { 1,  3,  5,  7, 18,  9, 16, 17, 18}, // start
**  1 { 2, 18, 18, 18, 18, 18, 18, 18, 18}, // FINAL &
**  2 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL &&
**  3 {18,  4, 18, 18, 18, 18, 18, 18, 18}, // FINAL |
**  4 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL ||
**  5 {18, 18,  6, 18, 18, 18, 18, 18, 18}, // FINAL ;
**  6 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL ;;
**  7 {12, 18, 18,  8, 18, 13, 18, 18, 18}, // FINAL <
**  8 {18, 18, 18, 18, 14, 18, 18, 18, 18}, //FINAL <<
**  9 {11, 15, 18, 18, 18, 10, 18, 18, 18}, // FINAL >
** 10 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL >>
** 11 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL >&
** 12 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL <&
** 13 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL <>
** 14 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL <<-
** 15 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL >|
** 16 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL (
** 17 {18, 18, 18, 18, 18, 18, 18, 18, 18}, //FINAL )
** 18 {18, 18, 18, 18, 18, 18, 18, 18, 18}  //error
**      &   |   ;   <   -   >  (   )   other
*/

static short	g_operator_transition_table[][21] =
{
	{ 1, 3, 5, 7, 18, 9, 16, 17, 18},
	{ 2, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 4, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 6, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{12, 18, 18, 8, 18, 13, 18, 18, 18},
	{18, 18, 18, 18, 14, 18, 18, 18, 18},
	{11, 15, 18, 18, 18, 10, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18},
	{18, 18, 18, 18, 18, 18, 18, 18, 18}
};

/*
** use dfa to detect if given input character can form the beginning
** of a valid operator as described by the grammar
*/

int		can_form_op(char c)
{
	short	category;
	int		next;

	category = g_operator_char_table[(int)c];
	next = g_operator_transition_table[0][category];
	next = OP_DFA_ACCEPTING(next);
	return (next);
}

/*
** use dfa to give next state of operator
*/

int		next_op_state(char c, int current)
{
	short	category;

	category = g_operator_char_table[(int)c];
	return ((int)g_operator_transition_table[current][category]);
}
