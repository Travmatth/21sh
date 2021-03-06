/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 19:30:37 by tmatthew         ###   ########.fr       */
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

int			accept_line(char **str);
int			accept(char **str, t_interface *ui, char **tmp, size_t len);
int			interface(char **str, char **tmp, t_interface *ui, int *end);

/*
** src/interface/ui_line.c
*/

int			init_uiline(t_interface *ui);
int			calculate_uilines(char *buf, t_interface *ui);

/*
** src/interface/utils.c
*/

void		free_uiline(t_uiline **ui_line);
int			get_cursor_position(int *x, int *y);
void		print_newline(void);

/*
** src/utils/history.c
*/

void		clear_all_lines(t_interface *ui);
void		write_to_history(char line[INPUT_LEN], t_interface *ui);
int			push_history(t_history **history, char *content);
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

int			move_index(unsigned long c, char *line, t_interface *ui);

/*
** src/interface/move_utils.c
*/

int			move_cursor(unsigned long c, t_interface *ui, int target);

/*
** src/interface/move_utils.c
*/

int			next_column(char *line, int index);
int			current_column(char *line, int index);
int			line_exists(t_interface *ui, int target, int direction);
t_uiline	*current_uiline(t_interface *ui, int target);

/*
** src/interface/modify_cli.c
*/

int			modify_cli(unsigned long c, t_interface *ui, char *line, int *cont);

/*
** src/interface/cut_copy_paste_utils.c
*/

int			violates_line_len(int insert, t_interface *ui);
void		write_line(t_interface *ui, char *line);
void		set_cursor(t_interface *ui, int position);

/*
** src/interface/select.c
*/

void		exit_select(t_interface *ui, char *line);
void		init_select(t_interface *ui);
void		select_ccp(unsigned long c, t_interface *ui, char *line);

/*
** src/interface/cut.c
*/

int			cut(t_interface *ui, char *buf, char *line);
int			cut_line(t_interface *ui, char *buf, char *line);

/*
** src/interface/paste.c
*/

int			paste(t_interface *ui, char *buf, char *line);
int			paste_line(t_interface *ui, char *buf, char *line);

/*
** src/interface/copy.c
*/

void		copy(t_interface *ui, char *buf, char *line);
void		copy_line(t_interface *ui, char *buf, char *line);

/*
** src/interface/insert_delete.c
*/

int			insert(char c, char **line, t_interface *ui, char **tmp);
int			delete(char *line, t_interface *ui);
#endif
