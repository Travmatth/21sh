/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 15:33:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/23 17:24:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GRAMMAR_H
# define GRAMMAR_H

# define AND_IF "&&"
# define OR_IF "||"
# define DSEMI ";;"
# define DLESS  "<<"
# define DGREAT ">>"
# define LESSAND "<&"
# define GREATAND ">&"
# define LESSGREAT "<>"
# define DLESSDASH "<<-"
# define CLOBBER ">|"
# define LBRACE "{"
# define RBRACE "}"
# define BANG "!"
# define SEMICOLON ";"
# define AMPERSAND "&"

/*
** Operator DFA definitions
*/

# define AND_IF_STATE 2
# define OR_IF_STATE 4
# define DSEMI_STATE 6
# define DLESS_STATE 8
# define DGREAT_STATE 10
# define LESSAND_STATE 11
# define GREATAND_STATE 12
# define LESSGREAT_STATE 13
# define DLESSDASH_STATE 14
# define CLOBBER_STATE 15
# define ACCEPTING(x) ((x >= 2 || x <= 10) && !(x % 2) || (x >= 11) || x <= 15)
# define NOT_ERR_STATE(x) (x != 15)

# define WORD 1
# define IO_HERE 2
#endif
