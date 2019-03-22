/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:29:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/21 13:59:06 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRAVERSE_H
# define TRAVERSE_H

# define EXIT_OK(x) (x == 0)

typedef struct	s_ectx
{
	int			in_fd;
	int			out_fd;
	int			err_fd;
	int			orig_in;
	int			orig_out;
	int			orig_err;
	int			is_bg;
	int			exit_code;
}				t_ectx;

int				prep_terminal_here_end(struct termios *tty);
int				restore_terminal_here_end(struct termios *tty);
int				execute_cmd(char **command);
void			set_exec_ctx(t_ectx *e_ctx);
void			reset_exec_ctx(t_ectx *e_ctx);
int				exec_simple_command(t_ectx *e_ctx, t_ast_node *root);
#endif
