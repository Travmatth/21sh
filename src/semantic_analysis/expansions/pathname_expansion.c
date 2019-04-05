/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:44:02 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/04 12:51:46 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Pathname Expansion
** After field splitting, if set -f is not in effect, each field in the
** resulting command line shall be expanded using the algorithm described in
** Pattern Matching Notation, qualified by the rules in Patterns Used for
** Filename Expansion.
*/

int		pathname_expansion(char ***fields)
{
	(void)fields;
	return (SUCCESS);
}
