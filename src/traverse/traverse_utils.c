/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 15:04:48 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/23 15:59:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	set_exec_ctx(t_ectx *e_ctx)
{
	e_ctx->is_bg = FALSE;
	e_ctx->in_fd = STDIN;
	e_ctx->out_fd = STDOUT;
	e_ctx->err_fd = STDERR;
	e_ctx->orig_in = ERROR;
	e_ctx->orig_out = ERROR;
	e_ctx->orig_err = ERROR;
}

void	reset_exec_ctx(t_ectx *e_ctx)
{
	e_ctx->is_bg = FALSE;
	if (!ERR(e_ctx->orig_in))
	{
		dup2(e_ctx->orig_in, STDIN);
		close(e_ctx->orig_in);
		e_ctx->in_fd = e_ctx->orig_in;
		e_ctx->orig_in = ERROR;
	}
	if (!ERR(e_ctx->orig_out))
	{
		dup2(e_ctx->orig_out, STDOUT);
		close(e_ctx->orig_out);
		e_ctx->out_fd = e_ctx->orig_out;
		e_ctx->orig_out = ERROR;
	}
	if (!ERR(e_ctx->orig_err))
	{
		dup2(e_ctx->orig_err, STDERR);
		close(e_ctx->orig_err);
		e_ctx->err_fd = e_ctx->orig_err;
		e_ctx->orig_err = ERROR;
	}
}

void	throw_redir_err(void)
{
}
