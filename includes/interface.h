/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 22:20:58 by tmatthew         ###   ########.fr       */
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

int			interface(char **line, t_interface *interface);
void		insert(char c, char **line, t_interface *interface);
void		delete(unsigned long c, char **line, t_interface *interface);

/*
** src/interface/utils.c
*/

int			get_cursor_position(int *x, int *y);
int			same_line(char *line, int current, int target, int step);

/*
** src/utils/history.c
*/

void		clear_all_lines(char *line, t_interface *ui);
void		write_to_history(char *line[INPUT_LEN]
						, t_interface *interface);
int			push_history(t_history **history, char *content);
void		print_history(t_interface *ui, char *line, char *next);
void		history(unsigned long c
				, char *line
				, t_h_list *h_list
				, t_interface *ui);

/*
** src/utils/history_utils.c
*/

void		init_history(t_h_list *h_list);
t_history	*init_h_node(char *content);

/*
** src/interface/move_index.c
*/

int			move_word_index_left(char *line, t_interface *ui);
int			move_word_index_right(char *line, t_interface *ui);
int			move_line_index_up(char *line, t_interface *ui);
int			move_line_index_down(char *line, t_interface *ui);
int			move_index(unsigned long c, char *line, t_interface *ui);

/*
** src/interface/move_utils.c
*/

void		move_word_cursor_left(t_interface *ui, int target);
void		move_word_cursor_right(t_interface *ui, int target);
void		move_line_cursor_up(char *line, t_interface *ui, int target);
void		move_line_cursor_down(char *line, t_interface *ui, int target);
int			move_cursor(unsigned long c
						, char *line
						, t_interface *ui
						, int target);

/*
** src/interface/move_utils.c
*/

int			next_column(char *line, int index);
int			current_column(char *line, int index);
int			line_exists(char *line, int index, int direction);

/*
** src/interface/cut_copy_paste.c
*/

int			modify_cli(unsigned long c, t_interface *ui, char *line);

/*
** src/interface/cut_copy_paste_utils.c
*/

void		write_line(t_interface *ui, char *line);
void		init_select(t_interface *ui);
void		set_cursor(t_interface *ui, char *line, int position);
#endif
