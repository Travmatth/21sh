/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 14:35:26 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/19 14:36:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Here-Document
** <<-
** allows redirection of lines contained in a shell input file, known as a
** "here-document", to the input of a command. The here-document shall be
** treated as a single word that begins after the next <newline> and continues
** until there is a line containing only the delimiter and a <newline>, with no
** <blank>s in between. Then the next here-document starts, if there is one. The
** format is as follows:
**    [n]<<-word
**        here-document
**    delimiter
** where the optional n represents the file descriptor number. If the number is
** omitted, the here-document refers to standard input (file descriptor 0).
** If any character in word is quoted, the delimiter shall be formed by
** performing quote removal on word, and the here-document lines shall not be
** expanded. Otherwise, the delimiter shall be the word itself.
** If no characters in word are quoted, all lines of the here-document shall be
** expanded for parameter expansion, command substitution, and arithmetic
** expansion. In this case, the backslash in the input behaves as the backslash
** inside double-quotes (see Double-Quotes). However, the double-quote character
** ( '"' ) shall not be treated specially within a here-document, except when
** the double-quote appears within "$()", "``", or "${}".
** If the redirection symbol is "<<-", all leading <tab>s shall be stripped from
** input lines and the line containing the trailing delimiter. If more than one
** "<<" or "<<-" operator is specified on a line, the here-document associated
** with the first operator shall be supplied first by the application and shall
** be read first by the shell.
*/

int		redir_heredoc_dash(t_redir *redir)
{
	int		status;

	(void)redir;
	status = NORMAL_CHILD_EXIT;
	return (status);
}

/*
** Heredoc
*/

int		redir_heredoc(t_redir *redir)
{
	return (redir->replacement == ERROR ? ERROR_CHILD_EXIT : NORMAL_CHILD_EXIT);
}
