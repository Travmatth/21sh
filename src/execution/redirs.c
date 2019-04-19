/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:22:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/18 19:17:00 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Redirecting Output
** [n]>word
** where the optional n represents the file descriptor number. If the number is
** omitted, the redirection shall refer to standard output (file descriptor 1).
** Output redirection using the '>' format shall fail if the noclobber option
** is set (see the description of set -C) and the file named by the expansion
** of word exists and is a regular file. Otherwise, redirection using the '>'
or ">|" formats shall cause the file whose name results from the expansion of
** word to be created and opened for output on the designated file descriptor,
** or standard output if none is specified. If the file does not exist, it
** shall be created; otherwise, it shall be truncated to be an empty file after
** being opened.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_out(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

/*
** Redirecting Input
** [n]<word
** where the optional n represents the file descriptor number. If the number is
** omitted, the redirection shall refer to standard input (file descriptor 0).
** Input redirection shall cause the file whose name results from the expansion
** of word to be opened for reading on the designated file descriptor, or
** standard input if the file descriptor is not specified.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_input(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
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

int		redir_heredoc(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

/*
** Appending Redirected Output
** [n]>>word
** where the optional n represents the file descriptor number. If the number
** is omitted, the redirection refers to standard output (file descriptor 1).
** Appended output redirection shall cause the file whose name results from
** the expansion of word to be opened for output on the designated file
** descriptor. The file is opened as if the open() function as defined in the
** System Interfaces volume of POSIX.1-2017 was called with the O_APPEND flag.
** If the file does not exist, it shall be created.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_out_append(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

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

int		redir_input_dup(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
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
** see Consequences of Shell Errors. If word evaluates to '-', file descriptor n,
** or standard output if n is not specified, is closed. Attempts to close a file
** descriptor that is not open shall not constitute an error. If word evaluates
** to something else, the behavior is unspecified.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_out_dup(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

/*
** Open File Descriptors for Reading and Writing
** [n]<>word
** shall cause the file whose name is the expansion of word to be opened for
** both reading and writing on the file descriptor denoted by n, or standard
** input if n is not specified. If the file does not exist, it shall be created.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_inout(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

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

int		redir_heredoc_dash(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}

/*
** Redirecting Output
** [n]>|word
** where the optional n represents the file descriptor number. If the number is
** omitted, the redirection shall refer to standard output (file descriptor 1).
** Redirection using the '>' or ">|" formats shall cause the file whose name
** results from the expansion of word to be created and opened for output on
** the designated file descriptor, or standard output if none is specified.
** If the file does not exist, it shall be created; otherwise, it shall be
** truncated to be an empty file after
** being opened.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_clobber(int fds[3], t_redir *redir)
{
	int		status;

	(void)fds;
	(void)redir;
	status = SUCCESS;
	return (status);
}


int		perform_redir(int fds[3], t_redir *redir)
{
	int		status;

	status = SUCCESS;
	if (redir->type == REDIR_GT)
		status = redir_out(fds, redir);
	else if (redir->type == REDIR_LT)
		status = redir_input(fds, redir);
	else if (redir->type == REDIR_DLESS)
		status = redir_heredoc(fds, redir);
	else if (redir->type == REDIR_DGREAT)
		status = redir_out_append(fds, redir);
	else if (redir->type == REDIR_LESSAND)
		status = redir_input_dup(fds, redir);
	else if (redir->type == REDIR_GREATAND)
		status = redir_out_dup(fds, redir);
	else if (redir->type == REDIR_LESSGREAT)
		status = redir_inout(fds, redir);
	else if (redir->type == REDIR_DLESSDASH)
		status = redir_heredoc_dash(fds, redir);
	else if (redir->type == REDIR_CLOBBER)
		status = redir_clobber(fds, redir);
	return (status);
}

int		perform_redirs(int fds[3], t_simple *simple)
{
	int		status;
	t_redir	*redir;

	status = SUCCESS;
	redir = simple->redirs;
	while (OK(status) && redir)
	{
		simple->exit_status = perform_redir(fds, redir);
		status = simple->exit_status;
	}
	return (status);
}
