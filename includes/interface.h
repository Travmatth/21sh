/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/10 16:10:47 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

# define IS_WHITESPACE(x) ((x == ' ' || x == '\t' || x == '\n'))

/*
** Ascii characters that will display on the tty
*/

# define PRINTABLE_CHAR(x) (x >= 32 && x <= 126)

/*
** String command used to query tty for cursor coordinates
*/

# define CURSOR_POS_REQ "\033[6n"

/*
** Data structure used to manage the position of the cursor, every segment
** is composed of a word and leading / trailing whitespaces.
*/

typedef struct s_uisegment t_uisegment;
struct			s_uisegment
{
	t_uisegment	*next;
	t_uisegment	*prev;
	int					x;
	int					y;
	int					start;
	int					end;
	int					word_end;
}						;

/*
** src/interface/interface.c
*/

int						interface(char **line);

/*
** src/interface/utils.c
*/

t_uisegment				*new_segment(void);
void					add_segment(t_uisegment **segments, t_uisegment *seg);
int						get_cursor_position(int *x, int *y);
#endif
