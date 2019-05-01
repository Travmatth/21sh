/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/30 12:21:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Here-Document
** <<
** allows redirection of lines contained in a shell input file, known as a
** "here-document", to the input of a command. The here-document shall be
** treated as a single word that begins after the next <newline> and continues
** until there is a line containing only the delimiter and a <newline>, with no
** <blank>s in between. Then the next here-document starts, if there is one. The ** format is as follows:
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
	char	next;
	char	*buf;
	char	*tmp;
	char	*line;
	size_t	here_end_len;
	size_t	buf_len;
	size_t	line_len;
	int		status;
	struct termios	tty[2];
	int		fd[2];
	int		found;

	buf_len = 0;
	line_len = 0;
	here_end_len = LEN(redir->word, 0);
	buf = ft_strnew(here_end_len);
	tmp = NULL;
	line = NULL;
	found = FALSE;
	status = ERR(prep_terminal(tty, ~(ICANON | ISIG | ECHO))) || ERR(pipe(fd)) ? ERROR : SUCCESS;
	redir->replacement = ERR(status) ? ERROR : fd[0];
	ft_putstr("heredoc > ");
	while(OK(status) && !found)
	{
		// read next char
		// signal encountered, break
		status = ERR(read(STDIN, &next, 1)) ? ERROR : status;
		status = next == INTR || next == EOT ? NIL : status;
		// no line, create
		if (OK(status) && !line && (!(line = ft_strnew(here_end_len))))
			status = ERROR;
		// if here_end found, stop next iteration
		if (OK(status) && next == '\n' && here_end_len == buf_len && IS_A(redir->word, buf))
		{
			found = TRUE;
			ft_putchar('\n');
		}
		// if buffer full or if newline encountered, write buffer to line
		if (OK(status) && !found && (here_end_len == buf_len || next == '\n'))
		{
			if (!(tmp = ft_strjoin(line, buf)))
				status = ERROR;
			free(line);
			line = tmp;
			tmp = NULL;
			line_len += buf_len;
			buf_len = 0;
			ft_bzero(buf, here_end_len);
		}
		// if newline encountered, write buf to line
		// if here_end quoted, write line to pipe 
		// if here_end not quoted, write expanded line to pipe 
		if (OK(status) && !found && next == '\n')
		{
			ft_putstr("\nheredoc > ");
			if (redir->heredoc_quoted)
				tmp = line;
			else
				status = heredoc_line_expansion(&tmp, line);
			if (OK(status)
				&& (ERR(write(fd[1], tmp, LEN(tmp, 0))) || ERR(write(fd[1], "\n", 1))))
				status = ERROR;
			free(tmp);
			tmp = NULL;
			line = NULL;
			line_len = 0;
			ft_bzero(buf, buf_len);
			buf_len = 0;
		}
		// if next backspace, remove last char
		else if (OK(status) && next == DEL)
		{
			if (buf_len)
			{
				buf[--buf_len] = '\0';
				status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
			}
			else if (line_len)
			{
				line[--line_len] = '\0';
				status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
			}
		}
		// if buffer not full, write next to buffer
		else if (OK(status) && !found && next != DEL)
		{
			buf_len += 1;
			ft_putchar(next);
			ft_strncat(buf, &next, 1);
		}
	}
	return (ERR(restore_terminal(&tty[1])) || ERR(close(fd[1])) ? ERROR : status);
}
