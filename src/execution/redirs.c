/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:22:21 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 16:08:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Detect type of current redirect and call appropriate preparation function
*/

int		redirs_switch(t_redir *redir)
{
	if (redir->type == REDIR_GT)
		return (redir_out(redir));
	else if (redir->type == REDIR_LT)
		return (redir_input(redir));
	else if (redir->type == REDIR_DLESS)
		return (redir_heredoc(redir));
	else if (redir->type == REDIR_DGREAT)
		return (redir_out_append(redir));
	else if (redir->type == REDIR_LESSAND)
		return (redir_input_dup(redir));
	else if (redir->type == REDIR_GREATAND)
		return (redir_out_dup(redir));
	else if (redir->type == REDIR_LESSGREAT)
		return (redir_inout(redir));
	else if (redir->type == REDIR_DLESSDASH)
		return (redir_heredoc_dash(redir));
	else if (redir->type == REDIR_CLOBBER)
		return (redir_clobber(redir));
	return (ERROR);
}

/*
** Iterate through specified redirections and prepare each
*/

int		open_redirs(t_simple *simple)
{
	t_redir	*redir;

	redir = simple->redirs;
	simple->exit_status = NORMAL_CHILD_EXIT;
	while (simple->exit_status == 0 && redir)
	{
		simple->exit_status = redirs_switch(redir);
		redir = redir->next;
	}
	return (simple->exit_status);
}

/*
** Perform specified redirections by opening specified file descriptor or
** closing current when appropriate
*/

int		perform_redirs(t_simple *simple)
{
	int		status;
	t_redir	*redir;

	errno = 0;
	status = NORMAL_CHILD_EXIT;
	redir = simple->redirs;
	while (IS_NORMAL_CHILD_EXIT(status) && redir)
	{
		if (IS_A("-", redir->word))
		{
			if (redir->type != REDIR_GREATAND && redir->type != REDIR_LESSAND)
				ft_printf("Execution error: invalid redirection\n");
			else
				status = close(redir->original);
		}
		else if (ERR(dup2(redir->replacement, redir->original)))
		{
			status = ERROR_CHILD_EXIT;
			if (errno == EBADF)
				ft_printf("Execution Error: %d: bad file descriptor\n"
					, redir->replacement);
		}
		redir = redir->next;
	}
	return (status);
}
