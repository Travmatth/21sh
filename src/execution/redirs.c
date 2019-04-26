/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:22:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/26 16:31:14 by tmatthew         ###   ########.fr       */
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

int		redir_out(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (ERR((redir->replacement = open(redir->word
									, O_WRONLY | O_TRUNC | O_CREAT
									, NEW_FILE_PERMS))))
	{
		ft_printf("Semantic Error: Could not open or create: %s\n", redir->word);
		status = ERROR_CHILD_EXIT;
	}
	ft_printf("Opened %s on fd %d\n", redir->word, redir->replacement);
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

int		redir_input(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (ERR((redir->replacement = open(redir->word, O_RDONLY))))
	{
		ft_printf("Semantic Error: No such file or directory: %s\n", redir->word);
		status = ERROR_CHILD_EXIT;
	}
	return (status);
}

/*
** Heredoc
*/

int		redir_heredoc(t_redir *redir)
{
	return (redir->replacement == ERROR ? ERROR_CHILD_EXIT : NORMAL_CHILD_EXIT);
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

int		redir_out_append(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (ERR((redir->replacement = open(redir->word, O_WRONLY | O_CREAT | O_APPEND, NEW_FILE_PERMS))))
		status = ERROR_CHILD_EXIT;
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

int		redir_input_dup(t_redir *redir)
{
	int		status;

	status = ERROR_CHILD_EXIT;
	if (IS_A("-", redir->word))
		status = close(redir->original);
	else if (ERR(ft_safeatoi(redir->word, &redir->replacement)))
		ft_printf("Execution Error: %s: ambiguous redirect");
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

int		redir_out_dup(t_redir *redir)
{
	int		status;

	status = ERROR_CHILD_EXIT;
	if (IS_A("-", redir->word))
		status = close(redir->original);
	else if (ERR(ft_safeatoi(redir->word, &redir->replacement)))
		ft_printf("Execution Error: %s: ambiguous redirect");
	ft_printf("Will duplicate fd %d on fd %d\n", redir->original, redir->replacement);
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

int		redir_inout(t_redir *redir)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (ERR((redir->replacement = open(redir->word, O_RDWR | O_CREAT, NEW_FILE_PERMS))))
		status = ERROR_CHILD_EXIT;
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

int		redir_heredoc_dash(t_redir *redir)
{
	int		status;

	(void)redir;
	status = NORMAL_CHILD_EXIT;
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

int		redir_clobber(t_redir *redir)
{
	int		status;

	(void)redir;
	status = NORMAL_CHILD_EXIT;
	return (status);
}

int		open_redirs(t_simple *simple)
{
	t_redir	*redir;

	redir = simple->redirs;
	simple->exit_status = NORMAL_CHILD_EXIT;
	while (simple->exit_status == 0 && redir)
	{
		if (redir->type == REDIR_GT)
			simple->exit_status = redir_out(redir);
		else if (redir->type == REDIR_LT)
			simple->exit_status = redir_input(redir);
		else if (redir->type == REDIR_DLESS)
			simple->exit_status = redir_heredoc(redir);
		else if (redir->type == REDIR_DGREAT)
			simple->exit_status = redir_out_append(redir);
		else if (redir->type == REDIR_LESSAND)
			simple->exit_status = redir_input_dup(redir);
		else if (redir->type == REDIR_GREATAND)
			simple->exit_status = redir_out_dup(redir);
		else if (redir->type == REDIR_LESSGREAT)
			simple->exit_status = redir_inout(redir);
		else if (redir->type == REDIR_DLESSDASH)
			simple->exit_status = redir_heredoc_dash(redir);
		else if (redir->type == REDIR_CLOBBER)
			simple->exit_status = redir_clobber(redir);
		redir = redir->next;
	}
	return (simple->exit_status);
}

int		perform_redirs(t_simple *simple)
{
	int		status;
	t_redir	*current;

	status = NORMAL_CHILD_EXIT;
	current = simple->redirs;
	while (IS_NORMAL_CHILD_EXIT(status) && current)
	{
		ft_printf("duplicating fd %d to fd %d\n", current->replacement, current->original);
		if (ERR(dup2(current->replacement, current->original)))
			status = ERROR_CHILD_EXIT;
		ft_printf("duplication: %d\n", status);
		current = current->next;
	}
	return (0);
}
