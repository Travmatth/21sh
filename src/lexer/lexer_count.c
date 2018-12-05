/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 18:45:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/04 12:34:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// if we encounter another opening candidate
// we need to restart search w/ that sequence

int		find_sub_end(char c, char *complete_cmd, size_t *j)
{
	(void)c;
	(void)complete_cmd;
	(void)j;
	return (SUCCESS);
}

int		escaped(char *complete_cmd, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (complete_cmd[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

int		op_ends_with(char *complete_cmd, int in_op, size_t i)
{
	char	*start;

	start = &complete_cmd[i - (size_t)in_op + 1];
	if (ft_strnequ(&AND_IF[1], start, LEN(&AND_IF[1], 0))
		|| ft_strnequ(&OR_IF[1], start, LEN(&OR_IF[1], 0))
		|| ft_strnequ(&DSEMI[1], start, LEN(&DSEMI[1], 0))
		|| ft_strnequ(&DLESS[1], start, LEN(&DLESS[1], 0))
		|| ft_strnequ(&DGREAT[1], start, LEN(&DGREAT[1], 0))
		|| ft_strnequ(&LESSAND[1], start, LEN(&LESSAND[1], 0))
		|| ft_strnequ(&GREATAND[1], start, LEN(&GREATAND[1], 0))
		|| ft_strnequ(&LESSGREAT[1], start, LEN(&LESSGREAT[1], 0))
		|| ft_strnequ(&DLESSDASH[1], start, LEN(&DLESSDASH[1], 0))
		|| ft_strnequ(&CLOBBER[1], start, LEN(&CLOBBER[1], 0))
		|| ft_strnequ(&LBRACE[1], start, LEN(&LBRACE[1], 0))
		|| ft_strnequ(&RBRACE[1], start, LEN(&RBRACE[1], 0))
		|| ft_strnequ(&BANG[1], start, LEN(&BANG[1], 0))
		|| ft_strnequ(&SEMICOLON[1], start, LEN(&SEMICOLON[1], 0))
		|| ft_strnequ(&AMPERSAND[1], start, LEN(&AMPERSAND[1], 0)))
		return (SUCCESS);
	return (NIL);
}

/*
** given compete_cmd with op starting at i, determine whether operator is valid
*/

int		can_form_op(char *complete_cmd, int in_op, size_t *i, int position)
{
	if (position == END)
		return (op_ends_with(complete_cmd, in_op, *i));
	else if (complete_cmd[*i] == AND_IF[0]
		|| complete_cmd[*i] == OR_IF[0]
		|| complete_cmd[*i] == DSEMI[0]
		|| complete_cmd[*i] == DLESS[0]
		|| complete_cmd[*i] == DGREAT[0]
		|| complete_cmd[*i] == LESSAND[0]
		|| complete_cmd[*i] == GREATAND[0]
		|| complete_cmd[*i] == LESSGREAT[0]
		|| complete_cmd[*i] == DLESSDASH[0]
		|| complete_cmd[*i] == CLOBBER[0]
		|| complete_cmd[*i] == LBRACE[0]
		|| complete_cmd[*i] == RBRACE[0]
		|| complete_cmd[*i] == BANG[0]
		|| complete_cmd[*i] == SEMICOLON[0]
		|| complete_cmd[*i] == AMPERSAND[0])
		return (SUCCESS);
	return (NIL);
}
