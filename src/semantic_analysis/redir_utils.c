/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:17:19 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/17 18:02:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Push a given t_redir to the end of the list of t_redir's on the given command
*/

int		push_redir(t_simple *cmd, t_redir *redir)
{
	t_redir	*r;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		r = cmd->redirs;
		while (r && r->next)
			r = r->next;
		r->next = redir;
	}
	return (SUCCESS);
}

/*
** Extract the char *string contained within a child ast_node
*/

int		get_filename(char **filename, t_ast_node *root)
{
	char	*name;

	name = NULL;
	if (!(name = ft_strdup(((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	*filename = name;
	return (name && name[0] ? SUCCESS : NIL);
}

/*
** Array listing parse symbols signifying redirection and their
** equivalent REDIR_{} type
*/

t_redir_cnv	g_redirs[TOTAL_REDIRS] =
{
	{ PARSE_GT, REDIR_GT },
	{ PARSE_LT, REDIR_LT },
	{ PARSE_DLESS, REDIR_DLESS },
	{ PARSE_DGREAT, REDIR_DGREAT },
	{ PARSE_LESSAND, REDIR_LESSAND },
	{ PARSE_GREATAND, REDIR_GREATAND },
	{ PARSE_LESSGREAT, REDIR_LESSGREAT },
	{ PARSE_DLESSDASH, REDIR_DLESSDASH },
	{ PARSE_CLOBBER, REDIR_CLOBBER }
};

/*
** Process given redirection into a struct to be pushed onto simple command
** redir queue, for later redirection during execution. Heredoc here_end words
** are subject to quote removal, while all other redirections subjected to
** tilde expansion, param expansion, command substitution, arithmetic expansion,
** and quote removal. >| and <<- redirections not supported. 
*/

int		process_redir(t_redir *redir, int io_num, char *filename, int type)
{
	int		i;
	int		status;
	char	*name;

	if (type == PARSE_CLOBBER || type == PARSE_DLESSDASH)
	{
		name = type == PARSE_CLOBBER ? ">|" : "<<-";
		ft_printf("Semantic Error: Redirection %s not implemented", name);
	}
	i = -1;
	while (++i < TOTAL_REDIRS)
	{
		if (g_redirs[i].parse_tok == type)
		{
			type = g_redirs[i].redir_tok;
			break ;
		}
	}
	status = (type == REDIR_DLESS || type == REDIR_DLESSDASH)
		? heredoc_expansion(&name, filename)
		: redir_expansion(&name, filename);
	if (OK(status))
	{
		redir->word = name;
		redir->original = io_num;
		redir->type = type;
	}
	return (status);
}

/*
** Create redirection struct that will set and pushed onto t_simple struct
*/

int		create_redir(t_redir **redir, t_ast_node *root, char **file)
{
	int		status;

	if (!(*redir = ft_memalloc(sizeof(t_redir)))
		|| !OK((status = get_filename(file, root->val[1]))))
		return (redir ? status : ERROR);
	(*redir)->original = ERROR;
	(*redir)->replacement = ERROR;
	(*redir)->type = ERROR;
	return (SUCCESS);
}
