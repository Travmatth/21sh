/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/05 15:43:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include "interface_constructs.h"

/*
** src/interface/interface.c
*/

int		interface(char **line, t_interface *interface);
void	history(unsigned long c
				, char **line
				, t_h_list *h_list
				, t_interface *interface);

/*
** src/interface/utils.c
*/

int		get_cursor_position(int *x, int *y);

/*
** src/utils/history.c
*/

void	clear_all_lines(char *line, t_interface *ui);
void	init_history(t_h_list *h_list);
int		push_history(t_history **history, char *content);
void	write_to_history(char **line, t_interface *interface);

/*
** src/interface/move.c
*/

void	movement(unsigned long c, char **line, t_interface *ui);

/*
** src/interface/move_utils.c
*/

int		next_column(char *line, int index);
int		current_column(char *line, int index);
int		line_exists(char *line, int index, int direction);
#endif
