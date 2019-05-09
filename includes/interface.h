/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/08 17:46:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

/*
** String command used to query tty for cursor coordinates
*/

# define CURSOR_POS_REQ "\033[6n"

/*
** Data structure used to manage the position of the cursor, every segment
** is composed of a word and leading / trailing whitespaces.
*/

typedef struct			s_uisegment
{
	struct s_uisemgent	*next;
	struct s_uisemgent	*prev;
	int					x;
	int					y;
	int					start;
	int					end;
	int					word_start;
	int					word_end;
	int					first;
}						t_uisegment;

/*
** src/interface/interface.c
*/

int						interface(char **line);

/*
** src/interface/utils.c
*/

t_uisegment				*new_segment(void);
int						get_cursor_position(int *x, int *y);
#endif
