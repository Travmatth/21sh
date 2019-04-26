/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:22:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/25 16:31:25 by tmatthew         ###   ########.fr       */
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

int		redir_heredoc(t_redir *redir)
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
	if (IS_NORMAL_CHILD_EXIT((status = prep_here_end(tty))))
		status = ERR(pipe(fd)) ? ERROR : status;
	redir->replacement = fd[0];
	ft_putstr("heredoc > ");
	while(IS_NORMAL_CHILD_EXIT(status) && !found)
	{
		// read next char
		// signal encountered, break
		if (ERR(read(STDIN, &next, 1)))
			status = ERROR;
		else if (next == INTR || next == EOT)
			status = ERROR_CHILD_EXIT;
		else
			status = NORMAL_CHILD_EXIT;
		// no line, create
		if (IS_NORMAL_CHILD_EXIT(status) && !line && (!(line = ft_strnew(here_end_len))))
			status = ERROR;
		// if here_end found, stop next iteration
		if (IS_NORMAL_CHILD_EXIT(status) && next == '\n' && here_end_len == buf_len && IS_A(redir->word, buf))
		{
			found = TRUE;
			ft_putchar('\n');
		}
		// if buffer full, write buffer to line
		if (IS_NORMAL_CHILD_EXIT(status) && !found && here_end_len == buf_len)
		{
			if (!(tmp = ft_strjoin(line, buf)))
				status = ERROR; free(line);
			line = tmp;
			tmp = NULL;
			line_len += buf_len;
			buf_len = 0;
			ft_bzero(buf, here_end_len);
		}
		// if newline encountered, write line && buffer to pipe
		if (IS_NORMAL_CHILD_EXIT(status) && !found && next == '\n')
		{
			ft_putstr("\nheredoc > ");
			if (ERR(write(fd[1], line, line_len))
				|| ERR(write(fd[1], buf, buf_len))
				|| ERR(write(fd[1], "\n", 1)))
				status = ERROR;
			free(line);
			line = NULL;
			line_len = 0;
			ft_bzero(buf, buf_len);
			buf_len = 0;
		}
		// if next backspace, remove last char
		else if (IS_NORMAL_CHILD_EXIT(status) && next == DEL)
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
		else if (IS_NORMAL_CHILD_EXIT(status) && !found && next != DEL)
		{
			buf_len += 1;
			ft_putchar(next);
			ft_strncat(buf, &next, 1);
		}
	}
	return (ERR(restore_here_end(fd[1], &tty[1])) ? ERROR : status);
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
		if (ERR(dup2(current->replacement, current->original)))
			status = ERROR_CHILD_EXIT;
		current = current->next;
	}
	return (0);
}
