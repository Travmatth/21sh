/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_dfa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 17:21:52 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/24 15:05:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** table to map ascii characters to their respective category
*/

static short	g_operator_char_table[128] =
{
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 0, 6,
	6, 6, 6, 6, 4, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 2, 3, 4, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 2, 6, 6, 6
};

/*
** table to specify allowable transitions between states,
** based on category of input character
**
**      &,  |,  ;,  <,  -,  >,  other
**  0 { 1,  3,  5,  7, 16,  9, 16}, // start
**  1 { 2, 16, 16, 16, 16, 16, 16}, // FINAL &
**  2 {16, 16, 16, 16, 16, 16, 16}, //FINAL &&
**  3 {16,  4, 16, 16, 16, 16, 16}, // FINAL |
**  4 {16, 16, 16, 16, 16, 16, 16}, //FINAL ||
**  5 {16, 16,  6, 16, 16, 16, 16}, // FINAL ;
**  6 {16, 16, 16, 16, 16, 16, 16}, //FINAL ;;
**  7 {12, 16, 16,  8, 16, 13, 16}, // FINAL <
**  8 {16, 16, 16, 16, 14, 16, 16}, //FINAL <<
**  9 {11, 15, 16, 16, 16, 10, 16}, // FINAL >
** 10 {16, 16, 16, 16, 16, 16, 16}, //FINAL >>
** 11 {16, 16, 16, 16, 16, 16, 16}, //FINAL >&
** 12 {16, 16, 16, 16, 16, 16, 16}, //FINAL <&
** 13 {16, 16, 16, 16, 16, 16, 16}, //FINAL <>
** 14 {16, 16, 16, 16, 16, 16, 16}, //FINAL <<-
** 15 {16, 16, 16, 16, 16, 16, 16}, //FINAL >|
** 16 {16, 16, 16, 16, 16, 16, 16} //error
**      &,  |,  ;,  <,  -,  >,  other
*/

static short	g_operator_transition_table[][17] =
{
	{ 1, 3, 5, 7, 16, 9, 16},
	{ 2, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 4, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 6, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{12, 16, 16, 8, 16, 13, 16},
	{16, 16, 16, 16, 14, 16, 16},
	{11, 15, 16, 16, 16, 10, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16},
	{16, 16, 16, 16, 16, 16, 16}
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
	return (ACCEPTING(next));
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
