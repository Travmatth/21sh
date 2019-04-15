/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_dfa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 17:21:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/13 18:40:24 by tmatthew         ###   ########.fr       */
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
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 0, 10,
	6, 7, 10, 10, 10, 4, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 2, 3, 10, 5, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, 10, 8, 1, 9, 10, 10
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
**      &   |   ;   <   -   >  (   )   {   }   other
**  0 { 1,  3,  5,  7, 20,  9, 110, 17, 18, 19, 20}, // start
**  1 { 2, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // FINAL &
**  2 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL &&
**  3 {20,  4, 20, 20, 20, 20, 20, 20, 20, 20, 20}, // FINAL |
**  4 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL ||
**  5 {20, 20,  6, 20, 20, 20, 20, 20, 20, 20, 20}, // FINAL ;
**  6 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL ;;
**  7 {12, 20, 20,  8, 20, 13, 20, 20, 20, 20, 20}, // FINAL <
**  8 {20, 20, 20, 20, 14, 20, 20, 20, 20, 20, 20}, //FINAL <<
**  9 {11, 15, 20, 20, 20, 10, 20, 20, 20, 20, 20}, // FINAL >
** 10 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL >>
** 11 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL >&
** 12 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL <&
** 13 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL <>
** 14 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL <<-
** 15 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL >|
** 16 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL (
** 17 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, //FINAL )
** 18 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}  //FINAL {
** 19 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}  //FINAL }
** 20 {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}  //error
**      &   |   ;   <   -   >  (   )   {   }   other
*/

static short	g_operator_transition_table[][21] =
{
	{ 1,  3,  5,  7, 20,  9, 16, 17, 18, 19, 20},
	{ 2, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20,  4, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20,  6, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{12, 20, 20,  8, 20, 13, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 14, 20, 20, 20, 20, 20, 20},
	{11, 15, 20, 20, 20, 10, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, 
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, 
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20},  
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}, 
	{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}
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
	next = ACCEPTING(next);
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
