/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orig_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 14:34:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/19 14:38:08 by tmatthew         ###   ########.fr       */
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
** or ">|" formats shall cause the file whose name results from the expansion of
** word to be created and opened for output on the designated file descriptor,
** or standard output if none is specified. If the file does not exist, it
** shall be created; otherwise, it shall be truncated to be an empty file after
** being opened.
** A failure to open or create a file shall cause a redirection to fail.
*/

int		redir_out(t_redir *redir)
{
	int		status;
	char	*message;

	status = NORMAL_CHILD_EXIT;
	message = "Semantic Error: Could not open or create: %s\n";
	if (ERR((redir->replacement = open(redir->word
									, O_WRONLY | O_TRUNC | O_CREAT
									, NEW_FILE_PERMS))))
	{
		ft_printf(message, redir->word);
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
	char	*message;

	status = NORMAL_CHILD_EXIT;
	message = "Semantic Error: No such file or directory: %s\n";
	if (ERR((redir->replacement = open(redir->word, O_RDONLY))))
	{
		ft_printf(message, redir->word);
		status = ERROR_CHILD_EXIT;
	}
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

int		redir_out_append(t_redir *redir)
{
	int		status;
	int		flags;

	status = NORMAL_CHILD_EXIT;
	flags = O_WRONLY | O_CREAT | O_APPEND;
	if (ERR((redir->replacement = open(redir->word, flags, NEW_FILE_PERMS))))
		status = ERROR_CHILD_EXIT;
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
	int		flags;

	flags = O_RDWR | O_CREAT;
	status = NORMAL_CHILD_EXIT;
	if (ERR((redir->replacement = open(redir->word, flags, NEW_FILE_PERMS))))
		status = ERROR_CHILD_EXIT;
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
