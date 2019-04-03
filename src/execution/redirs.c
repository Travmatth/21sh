/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:22:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 17:18:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Redirecting Input
** [n]<word
** where the optional n represents the file descriptor number. If the number is
** omitted, the redirection shall refer to standard input (file descriptor 0).
** Input redirection shall cause the file whose name results from the expansion
** of word to be opened for reading on the designated file descriptor, or
** standard input if the file descriptor is not specified.
*/
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
*/
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
*/
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
*/
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
*/
/*
** Open File Descriptors for Reading and Writing
** [n]<>word
** shall cause the file whose name is the expansion of word to be opened for
** both reading and writing on the file descriptor denoted by n, or standard
** input if n is not specified. If the file does not exist, it shall be created.
*/
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
*/
