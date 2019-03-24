/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   affixes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/23 16:38:47 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		process_io_file(int io_num, t_ectx *e_ctx, t_ast_node *root)
{
	int		tmp_fd;
	char	*filename;

	if (!ft_strcmp("< filename", root->rhs))
	{
		if (!ERR(e_ctx->orig_in))
		{
			dup2(e_ctx->orig_in, STDIN);
			close(e_ctx->orig_in);
			e_ctx->in_fd = e_ctx->orig_in;
			e_ctx->orig_in = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->in_fd : io_num;
		if (ERR((e_ctx->orig_in = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->in_fd);
		if (ERR((e_ctx->in_fd = open(filename, O_RDWR))))
			throw_redir_err();
	}
	else if (!ft_strcmp("LESSAND filename", root->rhs))
	{
		// if filename == '-', must close it
		ft_printf("'LESSAND filename' not implemented");
	}
	else if (!ft_strcmp("> filename", root->rhs))
	{
		if (!ERR(e_ctx->orig_out))
		{
			dup2(e_ctx->orig_out, STDOUT);
			close(e_ctx->orig_out);
			e_ctx->out_fd = e_ctx->orig_out;
			e_ctx->orig_out = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->out_fd : io_num;
		if (ERR((e_ctx->orig_out = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->out_fd);
		if (ERR((e_ctx->out_fd = open(filename, O_RDWR | O_CREAT, 0666))))
			throw_redir_err();
	}
	else if (!ft_strcmp("GREATAND filename", root->rhs))
	{
		// if filename == '-', must close it
		ft_printf("'GREATAND filename' not implemented");
	}
	else if (!ft_strcmp("DGREAT filename", root->rhs))
	{
		if (!ERR(e_ctx->orig_out))
		{
			dup2(e_ctx->orig_out, STDOUT);
			close(e_ctx->orig_out);
			e_ctx->out_fd = e_ctx->orig_out;
			e_ctx->orig_out = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->out_fd : io_num;
		if (ERR((e_ctx->orig_out = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->out_fd);
		if (ERR((e_ctx->out_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666))))
			throw_redir_err();
	}
	else if (!ft_strcmp("LESSGREAT filename", root->rhs))
	{
		ft_printf("'LESSGREAT filename' not implemented");
	}
	else if (!ft_strcmp("CLOBBER filename", root->rhs))
	{
		ft_printf("'CLOBBER filename' not implemented");
	}
	return (SUCCESS);
}

int		process_io_number(int *io_num, t_ast_node *root)
{
	char	*io;

	io = (char*)root->val[0];
	if (!ft_strcmp("0", io))
		*io_num = 0;
	else
	{
		if (NONE((*io_num = ft_atoi(io))))
			return (ERROR);
	}
	return (SUCCESS);
}

int		process_io_redirect(t_ectx *e_ctx, t_ast_node *root)
{
	int		io_num;

	io_num = ERROR;
	if (!ft_strcmp("io_file", root->rhs))
		return (process_io_file(io_num, e_ctx, root->val[0]));
	else if (!ft_strcmp("IO_NUMBER io_file", root->rhs))
	{
		if (ERR(process_io_number(&io_num, root->val[0])))
			return (ERROR);
		return (process_io_file(io_num, e_ctx, root->val[1]));
	}
	else if (!ft_strcmp("io_here", root->rhs))
		return (process_io_here(io_num, e_ctx, root->val[0]));
	else if (!ft_strcmp("IO_NUMBER io_here", root->rhs))
	{
		if (ERR(process_io_number(&io_num, root->val[0])))
			return (ERROR);
		return (process_io_here(io_num, e_ctx, root->val[1]));
	}
	return (SUCCESS);
}

int		process_prefix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	(void)command;
	(void)root;
	return (SUCCESS);
}

int		process_suffix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	char	**new;

	if (!ft_strcmp("io_redirect", root->rhs))
		return (process_io_redirect(e_ctx, root->val[0]));
	else if (!ft_strcmp("cmd_suffix io_redirect", root->rhs))
	{
		if (ERR(process_suffix(e_ctx, command, root->val[0])))
			return (ERROR);
		return (process_io_redirect(e_ctx, root->val[1]));
	}
	else if (!ft_strcmp("WORD", root->rhs))
	{
		new = ft_strappend(*command, ((t_ast_node*)root->val[0])->val[0]);
		free(*command);
		*command = new;
	}
	else if (!ft_strcmp("cmd_suffix WORD", root->rhs))
	{
		if (ERR(process_suffix(e_ctx, command, root->val[0])))
			return (ERROR);
		new = ft_strappend(*command, ((t_ast_node*)root->val[1])->val[0]);
		free(*command);
		*command = new;
	}
	return (SUCCESS);
}
