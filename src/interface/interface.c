/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/19 00:55:41 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// static t_line *init(char c)
// {
// 	t_line	*new;

// 	if (!(new = (t_line*)ft_memalloc(sizeof(t_line))))
// 		return (NULL);
// 	get_cursor_position(&new->x, &new->y);
// 	new->c = c;
// 	new->highlight = 0;
// 	new->next = NULL;
// 	new->prev = NULL;
// 	return (new);
// }

// static void insert(t_interface *interface)
// {
// 	t_line	*new;
	
// 	new = init()
// }

// static void delete(t_interface *interface)
// {
	
// }

// static void	move()
// {
	
// }

/*
** Read single character from STDIN. Break reading on CTRL-C and delete input
** on BACKSPACE, otherwise append character to line and echo to STDOUT
*/

int		interface(char **line)
{
	static char		tmp[4096];
	unsigned long	next;
	struct termios	tty[2];
	int				status;
	size_t			len;
	// t_uisegment		**segments;
	// t_uisegment		*current;
	int				in_word;

	len = 0;
	*line = tmp;
	status = prep_terminal(tty, ~(ICANON | ISIG | ECHO));
	// segments = NULL;
	in_word = FALSE;
	while (OK(status))
	{
		next = 0;
		read(STDIN, &next, 6);
		if (CTL_UP == next)
			ft_printf("next = CTL_UP = %#llx\n", next);
		// if char is newline or CTRL-C, end current line
		if (next == INTR || next == RETURN)
		{
			status = write(STDOUT, "\n", 1);
			exit(0) ;
		}
		// if char is delete, remove last char from line and STDOUT
		else if (next == DEL && len)
		{
			(*line)[--len] = '\0';
			status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
		}
		else if (next == DEL && !len)
			continue ;
		if (PRINTABLE_CHAR(next)){
			// puts("hi\n");
			status = ERR(write(STDOUT, (char*)&next, 1)) ? ERROR : status;
		}
		// else add char to line and STDOUT
		// else if ((tmp = ft_strjoin(*line, next)))
		// {
		// 	// if char is start of new word, add new segment to chain
		// 	if (!in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
		// 	{
		// 		if (current)
		// 			current->end = len - 1; 
		// 		current = new_segment();
		// 		add_segment(segments, current);
		// 		current->start = len;
		// 		in_word = TRUE;
		// 		if (ERR((status = get_cursor_position(&current->x, &current->y))))
		// 			break ;
		// 	}
		// 	// if char is whitespace and not in word, add blank segment to chain
		// 	if (!in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
		// 	{
		// 		if (current)
		// 			current->end = (int)len - 1; 
		// 		current = new_segment();
		// 		add_segment(segments, current);
		// 		current->start = (int)len;
		// 		if (ERR((status = get_cursor_position(&current->x, &current->y))))
		// 			break ;
		// 	}
			// if in word and char is not whitespace, continue current word
			// if (in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
			// {
			// }
			// if in word and char is whitespace, end current word
			// if (in_word && PRINTABLE_CHAR(next) && IS_WHITESPACE(next))
			// {
			// 	current->word_end = len - current->start;
			// 	in_word = FALSE;
			// }

			// if char is escaped newline, write prompt 
			if (next == RETURN && len && (*line)[len - 1] == '\\')
			{
				status = ERR(write(STDOUT, "\n> ", 3)) ? ERROR : status;
				len = 0;
			}
			else
			free(*line);
			*line = tmp;
			len += 1;
			
		}
		else
			status = ERROR;
	}
	return (ERR(restore_terminal(&tty[1])) ? ERROR : status);
}
// int		interface(char **line)
// {
// 	char			*tmp;
// 	char			next[2];
// 	struct termios	tty[2];
// 	int				status;
// 	size_t			len;
// 	t_uisegment		**segments;
// 	t_uisegment		*current;
// 	int				in_word;

// 	len = 0;
// 	next[1] = '\0';
// 	if (!*line && !(*line = ft_strnew(0)))
// 		return (ERROR);
// 	status = prep_terminal(tty, ~(ICANON | ISIG | ECHO));
// 	segments = NULL;
// 	in_word = FALSE;
// 	while (OK(status) && !ERR((status = read(STDIN, &next, 1))))
// 	{
// 		// if char is newline or CTRL-C, end current line
// 		if (next[0] == INTR || next[0] == RETURN)
// 		{
// 			status = write(STDOUT, "\n", 1);
// 			break ;
// 		}
// 		// if char is delete, remove last char from line and STDOUT
// 		else if (next[0] == DEL && len)
// 		{
// 			(*line)[--len] = '\0';
// 			status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
// 		}
// 		else if (next[0] == DEL && !len)
// 			continue ;
// 		// else add char to line and STDOUT
// 		else if ((tmp = ft_strjoin(*line, next)))
// 		{
// 			// if char is start of new word, add new segment to chain
// 			if (!in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			{
// 				if (current)
// 					current->end = len - 1; 
// 				current = new_segment();
// 				add_segment(segments, current);
// 				current->start = len;
// 				in_word = TRUE;
// 				if (ERR((status = get_cursor_position(&current->x, &current->y))))
// 					break ;
// 			}
// 			// if char is whitespace and not in word, add blank segment to chain
// 			if (!in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			{
// 				if (current)
// 					current->end = (int)len - 1; 
// 				current = new_segment();
// 				add_segment(segments, current);
// 				current->start = (int)len;
// 				if (ERR((status = get_cursor_position(&current->x, &current->y))))
// 					break ;
// 			}
// 			// if in word and char is not whitespace, continue current word
// 			// if (in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			// {
// 			// }
// 			// if in word and char is whitespace, end current word
// 			if (in_word && PRINTABLE_CHAR(next[0]) && IS_WHITESPACE(next[0]))
// 			{
// 				current->word_end = len - current->start;
// 				in_word = FALSE;
// 			}

// 			// if char is escaped newline, write prompt 
// 			if (next[0] == RETURN && len && (*line)[len - 1] == '\\')
// 			{
// 				status = ERR(write(STDOUT, "\n> ", 3)) ? ERROR : status;
// 				len = 0;
// 			}
// 			else
// 				status = ERR(write(STDOUT, next, 1)) ? ERROR : status;
// 			free(*line);
// 			*line = tmp;
// 			len += 1;
			
// 		}
// 		else
// 			status = ERROR;
// 	}
// 	return (ERR(restore_terminal(&tty[1])) ? ERROR : status);
// }
