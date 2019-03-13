/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:29:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/12 15:07:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRAVERSE_H
# define TRAVERSE_H

typedef struct	s_ectx
{
	int			in_fd;
	int			out_fd;
	int			err_fd;
	int			is_bg;
}				t_ectx;

void			reset_exec_ctx(t_ectx *e_ctx);
#endif
