/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_here_end.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 13:07:55 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/21 13:20:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		here_end_found(void)
{
	ft_putchar('\n');
	return (TRUE);
}

/*
** Here-Document
** <<
** allows redirection of lines contained in a shell input file, known as a
** "here-document", to the input of a command. The here-document shall be
** treated as a single word that begins after the next <newline> and continues
** until there is a line containing only the delimiter and a <newline>, with no
** <blank>s in between. Then the next here-document starts, if there is one. The
** format is as follows:
**    [n]<<word
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
** If more than one "<<" or "<<-" operator is specified on a line, the
** here-document associated with the first operator shall be supplied first by
** the application and shall be read first by the shell.
*/

int		process_heredoc(t_redir *redir)
{
	char			next;
	int				status;
	struct termios	tty[2];
	int				found;
	t_heredoc		heredoc;

	status = init_heredoc(redir, &heredoc, tty, &found);
	while (OK(status) && !found)
	{
		status = ERR(read(STDIN, &next, 1)) ? ERROR : status;
		status = next == INTR || next == EOT ? NIL : status;
		if (OK(status) && CREATE_LINE(heredoc))
			status = ERROR;
		if (OK(status) && HERE_END_FOUND(next, heredoc, redir))
			found = here_end_found();
		if (OK(status) && BUF_FULL(found, heredoc, next))
			status = handle_full_buffer(&heredoc);
		if (OK(status) && !found && next == '\n')
			status = add_newline_char(redir, &heredoc);
		else if (OK(status) && next == DEL)
			status = remove_last_char(&heredoc);
		else if (OK(status) && !found && next != DEL)
			add_next_char(&heredoc, &next);
	}
	return (RESTORE_HERE_END(heredoc, tty) ? ERROR : status);
}
