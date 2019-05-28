/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 14:33:56 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/27 17:03:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Duplicating an Input File Descriptor
** [n]<&word
** shall duplicate one input file descriptor from another, or shall close one.
** If word evaluates to one or more digits, the file descriptor denoted by n,
** or standard input if n is not specified, shall be made to be a copy of the
** file descriptor denoted by word; if the digits in word do not represent a
** file descriptor already open for input, a redirection error shall result;
** see Consequences of Shell Errors. If word evaluates to '-',
** file descriptor n, or standard input if n is not specified, shall be closed.
** Attempts to close a file descriptor that is not open shall not constitute an
** error. If word evaluates to something else, the behavior is unspecified.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_input_dup(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (ERR(ft_safeatoi(redir->word, &redir->replacement)))
	{
		ft_printf("Execution Error: %s: ambiguous redirect");
		status = ERROR_CHILD_EXIT;
	}
	return (status);
}

/*
** Duplicating an Output File Descriptor
** [n]>&word
** shall duplicate one output file descriptor from another, or shall close one.
** If word evaluates to one or more digits, the file descriptor denoted by n,
** or standard output if n is not specified, shall be made to be a copy of the
** file descriptor denoted by word; if the digits in word do not represent a
** file descriptor already open for output, a redirection error shall result;
** If word evaluates to '-', file descriptor n, or standard output if n is not
** specified, is closed. Attempts to close a file descriptor that is not open
** shall not constitute an error. If word evaluates to something else, the
** behavior is unspecified. A failure to open or create a file shall cause a
** redirection to fail.
*/

int		redir_out_dup(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (!IS_A("-", redir->word)
		&& ERR(ft_safeatoi(redir->word, &redir->replacement)))
	{
		ft_printf("Execution Error: %s: ambiguous redirect");
		status = ERROR_CHILD_EXIT;
	}
	return (status);
}
