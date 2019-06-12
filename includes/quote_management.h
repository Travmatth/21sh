/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 14:37:57 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 23:02:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_MANAGEMENT_H
# define QUOTE_MANAGEMENT_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include "quote_constructs.h"

/*
** src/utils/cmd_quoting.c
*/

int		first_word(char *str, int i);
int		parse_all(char **str, size_t start, size_t *end, t_quote f);
int		subshell(char **str, size_t start, size_t *end, t_quote f);
int		cur_shell(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/compound_quoting.c
*/

int		dbl_quote(char **str, size_t start, size_t *end, t_quote f);
int		uparam_exp(char **str, size_t start, size_t *end, t_quote f);
int		eparam_exp(char **str, size_t start, size_t *end, t_quote f);
int		command_sub(char **str, size_t start, size_t *end, t_quote f);
int		arith_exp(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/simple_quoting.c
*/

int		backslash(char **str, size_t start, size_t *end, t_quote f);
int		quote(char **str, size_t start, size_t *end, t_quote f);
int		backtick(char **str, size_t start, size_t *end, t_quote f);

/*
** src/utils/quoting_utils.c
*/

int		init_exp(size_t *i, size_t start, int *found);
int		dbl_quote_switch(size_t *end, int *found, size_t len);
int		param_exp_end(size_t *end, size_t len);
int		arith_exp_switch(size_t *end, size_t len, int *found);
int		switch_arith_exp(char **str, size_t i, size_t *count, int *found);
#endif
