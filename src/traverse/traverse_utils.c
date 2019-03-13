/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 15:04:48 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/12 15:06:43 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	reset_exec_ctx(t_ectx *e_ctx)
{
	e_ctx->is_bg = FALSE;
	e_ctx->in_fd = STDIN;
	e_ctx->out_fd = STDOUT;
	e_ctx->err_fd = STDERR;
}
